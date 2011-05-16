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
#include "dependencyjob.h"

#include <qlistview.h>
#include <qregexp.h>

DependencyJob::DependencyJob( QListViewItem *pItem, const QMap<QString, QString> *pLDDMap ) :
	m_pItem( pItem ),
	m_pLDDMap( pLDDMap )
{
	connect( &m_proc, SIGNAL( readyReadStdout() ), this, SLOT( readLineStdout() ) );
	connect( &m_proc, SIGNAL( processExited() ), this, SLOT( readLineStdout() ) );

	UnusedDependenciesJob *pJob = new UnusedDependenciesJob( pItem->text( 2 ), &m_unusedMap );
	connect( pJob, SIGNAL( finished() ), this, SLOT( finishedUnusedDependenciesJob() ) );
}

void DependencyJob::finishedUnusedDependenciesJob()
{
	m_proc.addArgument( "objdump" );
	m_proc.addArgument( "-p" );
	m_proc.addArgument( m_pItem->text( 2 ) );

	m_proc.start();
}

void DependencyJob::readLineStdout()
{
	while ( m_proc.canReadLineStdout() )
	{
		QString line = m_proc.readLineStdout();

		QRegExp re( "\\s+NEEDED\\s+(\\S+)" );
		if ( re.search( line ) >= 0 )
		{
			QListViewItem *pItem = new QListViewItem( m_pItem );

			pItem->setText( 0, re.cap( 1 ) );
			pItem->setText( 2, (*m_pLDDMap)[ re.cap( 1 ) ] );
			if ( m_unusedMap.contains( (*m_pLDDMap)[ re.cap( 1 ) ] ) )
			{
			//	pItem->setEnabled( false );
			}
			else
				pItem->setExpandable( true );
		}
	}

	if ( ! m_proc.isRunning() )
	{
		if ( m_pItem->childCount() == 0 )
			m_pItem->setExpandable( false );

		emit finished();
		disconnect( this, 0, 0, 0 );
		deleteLater();
	}
}


DependencyJob::UnusedDependenciesJob::UnusedDependenciesJob( const QString &file, QMap<QString, QString> *pUnusedMap ) :
	m_pUnusedMap( pUnusedMap )
{
	connect( &m_proc, SIGNAL( readyReadStdout() ), this, SLOT( readLineStdout() ) );
	connect( &m_proc, SIGNAL( processExited() ), this, SLOT( readLineStdout() ) );

	m_proc.addArgument( "ldd" );
	m_proc.addArgument( "-u" );
	m_proc.addArgument( file );

	m_proc.start();
}


void DependencyJob::UnusedDependenciesJob::readLineStdout()
{
	while ( m_proc.canReadLineStdout() )
	{
		QString line = m_proc.readLineStdout();

		QRegExp re( "\\s+(\\S+)" );
		if ( re.search( line ) >= 0 )
			(*m_pUnusedMap)[ re.cap( 1 ) ] = QFileInfo( re.cap( 1 ) ).fileName();
	}

	if ( ! m_proc.isRunning() )
	{
		emit finished();
		disconnect( this, 0, 0, 0 );
		deleteLater();
	}
}
