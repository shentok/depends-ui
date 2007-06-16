/***************************************************************************
 *   Copyright (C) 2007 by Bernhard Beschow   *
 *   bbeschow (.at) cs.tu-berlin.de   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "dependsui.h"

#include "initjob.h"
#include "importsexportsjob.h"
#include "dependencyjob.h"

#include <qfiledialog.h>
#include <qlistview.h>
#include <qregexp.h>

#include <iostream>

DependsUI::DependsUI ( QWidget* parent, const char* name, WFlags fl ) :
	DependsUIBase ( parent,name,fl )
{
}

DependsUI::~DependsUI()
{}

/*$SPECIALIZATION$*/
void DependsUI::fileExit()
{}

void DependsUI::fileOpen()
{
	QFileDialog dlg( this );

	if ( ! m_dirPath.isEmpty() )
		dlg.setDir( QDir( m_dirPath ) );

	if ( dlg.exec() == QDialog::Accepted )
	{
		m_dirPath = dlg.dirPath();
		openFile( dlg.selectedFile() );
	}
}

#include "dependsui.moc"

void DependsUI::openFile( const QString &file )
{
	setCaption( "Dependency Walker - [" + QFileInfo( file ).fileName() + "]" );
	m_pTreeSharedObjects->clear();
	m_pListSummarySharedObjects->clear();
	m_pListImports->clear();
	m_pListExports->clear();

	QListViewItem *pItem = new QListViewItem( m_pTreeSharedObjects );
	pItem->setText( 0, QFileInfo( file ).fileName() );
	pItem->setText( 2, file );
	pItem->setExpandable( true );

	new InitJob( file, &m_lddMap );
}

void DependsUI::dependencyExpanded( QListViewItem *pItem )
{
	if ( pItem->childCount() > 0 )
		return;

	new DependencyJob( pItem, &m_lddMap );
}

void DependsUI::dependencySelected( QListViewItem *pItem )
{
	new ImportsExportsJob( pItem, m_pListImports, m_pListExports );
}
