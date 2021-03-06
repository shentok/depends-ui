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

#include "importsexportsjob.h"
#include "dependencyjob.h"

#include <QAction>
#include <QFileDialog>
#include <QHeaderView>
#include <QTreeWidgetItem>


DependsUI::DependsUI( QWidget *parent, Qt::WindowFlags flags ) :
	QMainWindow( parent, flags ),
	m_ui()
{
	m_ui.setupUi( this );
	m_ui.m_pListImports->header()->setResizeMode( QHeaderView::ResizeToContents );
	m_ui.m_pListImports->setSortingEnabled( true );
	m_ui.m_pListExports->header()->setResizeMode( QHeaderView::ResizeToContents );
	m_ui.m_pListExports->setSortingEnabled( true );
	m_ui.m_pTreeSharedObjects->header()->setResizeMode( QHeaderView::ResizeToContents );

	connect( m_ui.fileOpenAction, SIGNAL(activated()), this, SLOT(fileOpen()) );
	connect( m_ui.fileExitAction, SIGNAL(activated()), this, SLOT(fileExit()) );
	connect( m_ui.m_pTreeSharedObjects, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(dependencyExpanded(QTreeWidgetItem*)) );
	connect( m_ui.m_pTreeSharedObjects, SIGNAL(itemActivated(QTreeWidgetItem*,int)), this, SLOT(dependencySelected(QTreeWidgetItem*)) );
}

DependsUI::~DependsUI()
{}

void DependsUI::fileExit()
{
	close();
}

void DependsUI::fileOpen()
{
	QFileDialog dlg( this );

	if ( ! m_dirPath.isEmpty() )
		dlg.setDirectory( QDir( m_dirPath ) );

	if ( dlg.exec() != QDialog::Accepted )
	{
		return;
	}

	m_dirPath = dlg.directory().path();

	const QStringList selectedFiles = dlg.selectedFiles();

	if ( selectedFiles.isEmpty() )
	{
		return;
	}

	openFile( selectedFiles.first() );
}

void DependsUI::openFile( const QString &file )
{
	setWindowTitle( tr( "Dependency Walker - [%1]" ).arg( QFileInfo( file ).fileName() ) );
	m_ui.m_pTreeSharedObjects->clear();
	m_ui.m_pListSummarySharedObjects->clear();
	m_ui.m_pListImports->clear();
	m_ui.m_pListExports->clear();

	QTreeWidgetItem *pItem = new QTreeWidgetItem( m_ui.m_pTreeSharedObjects );
	pItem->setText( 0, QFileInfo( file ).fileName() );
	pItem->setText( 1, file );

	DependencyJobs *job = new DependencyJobs( pItem, this );
	connect( job, SIGNAL(finished()), job, SLOT(deleteLater()) );
	connect( job, SIGNAL(finished()), this, SLOT(setRootItemExpanded()) );
}

void DependsUI::dependencyExpanded( QTreeWidgetItem *pItem )
{
	for ( int i = 0; i < pItem->childCount(); ++i )
	{
		QTreeWidgetItem *childItem = pItem->child( i );
		DependencyJobs *job = new DependencyJobs( childItem, this );
		connect( job, SIGNAL(finished()), job, SLOT(deleteLater()) );
	}
}

void DependsUI::dependencySelected( QTreeWidgetItem *pItem )
{
	ImportsExportsJob *job = new ImportsExportsJob( pItem, m_ui.m_pListImports, m_ui.m_pListExports );
	connect( job, SIGNAL(finished()), job, SLOT(deleteLater()) );
}

void DependsUI::setRootItemExpanded()
{
	m_ui.m_pTreeSharedObjects->topLevelItem( 0 )->setExpanded( true );
}
