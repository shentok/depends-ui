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
#include "importsexportsjob.h"

#import <qlistview.h>
#include <qregexp.h>

#include <iostream>

ImportsExportsJob::ExportsJob::ExportsJob( const QString &file, QListView *pExports, QMap<QString, QString> *pMap ) :
	m_pExports( pExports ),
	m_pExportsMap( pMap )
{
	connect( &m_proc, SIGNAL( readyReadStdout() ), this, SLOT( readLineStdout() ) );
	connect( &m_proc, SIGNAL( processExited() ), this, SLOT( readLineStdout() ) );

	m_proc.addArgument( "objdump" );
	m_proc.addArgument( "-TC" );
	m_proc.addArgument( file );
	m_proc.start();
}


void ImportsExportsJob::ExportsJob::readLineStdout()
{
	while ( m_proc.canReadLineStdout() )
	{
		QString line = m_proc.readLineStdout();

		QRegExp re( "^([0-9a-f]{8})\\s(.)(.)(.)(.)(.)(.)(.)\\s(\\S+)\t([0-9a-f]{8})\\s\\s?(\\s{11}|\\S+\\s*)\\s(\\S.*)$" );
		if ( re.search( line ) >= 0 )
		{
			if ( re.cap( 9 ).compare( "*UND*" ) == 0 )
				continue;

			QListViewItem *pItem = new QListViewItem( m_pExports );

			pItem->setText( 0, re.cap( 1 ) );
			pItem->setText( 1, re.cap( 2 ) );
			pItem->setText( 2, re.cap( 3 ) );
			pItem->setText( 3, re.cap( 4 ) );
			pItem->setText( 4, re.cap( 5 ) );
			pItem->setText( 5, re.cap( 6 ) );
			pItem->setText( 6, re.cap( 7 ) );
			pItem->setText( 7, re.cap( 8 ) );
			pItem->setText( 8, re.cap( 9 ) );
			pItem->setText( 9, re.cap( 10 ) );
			pItem->setText( 10, re.cap( 11 ) );
			pItem->setText( 11, re.cap( 12 ) );

			(*m_pExportsMap)[ re.cap( 12 ) ] = line;
		}
	}

	if ( ! m_proc.isRunning() )
	{
		for ( int i = 0; i < 12; ++i )
			m_pExports->adjustColumn( i );

		emit finished();
		disconnect( this, 0, 0, 0 );
		deleteLater();
	}
}


ImportsExportsJob::ImportsJob::ImportsJob( const QString &file, QListView *pImports, const QMap<QString, QString> *pMap ) :
	m_pImports( pImports ),
	m_pImportsMap( pMap )
{
	connect( &m_proc, SIGNAL( readyReadStdout() ), this, SLOT( readLineStdout() ) );
	connect( &m_proc, SIGNAL( processExited() ), this, SLOT( readLineStdout() ) );

	m_proc.addArgument( "objdump" );
	m_proc.addArgument( "-TC" );
	m_proc.addArgument( file );
	m_proc.start();
}


void ImportsExportsJob::ImportsJob::readLineStdout()
{
	while ( m_proc.canReadLineStdout() )
	{
		QString line = m_proc.readLineStdout();

		QRegExp re( "^([0-9a-f]{8})\\s(.)(.)(.)(.)(.)(.)(.)\\s(\\S+)\t([0-9a-f]{8})\\s\\s?(\\s{11}|\\S+\\s*)\\s(\\S.*)$" );
		if ( re.search( line ) >= 0 )
		{
			if ( re.cap( 9 ).compare( "*UND*" ) != 0 )
				continue;

			if ( m_pImportsMap->contains( re.cap( 12 ) ) )
			{
//				re.search( (*m_pImportsMap)[ re.cap( 12 ) ] );
				QListViewItem *pItem = new QListViewItem( m_pImports );

				pItem->setText( 0, re.cap( 1 ) );
				pItem->setText( 1, re.cap( 2 ) );
				pItem->setText( 2, re.cap( 3 ) );
				pItem->setText( 3, re.cap( 4 ) );
				pItem->setText( 4, re.cap( 5 ) );
				pItem->setText( 5, re.cap( 6 ) );
				pItem->setText( 6, re.cap( 7 ) );
				pItem->setText( 7, re.cap( 8 ) );
				pItem->setText( 8, re.cap( 9 ) );
				pItem->setText( 9, re.cap( 10 ) );
				pItem->setText( 10, re.cap( 11 ) );
				pItem->setText( 11, re.cap( 12 ) );
			}
		}
	}

	if ( ! m_proc.isRunning() )
	{
		for ( int i = 0; i < 12; ++i )
			m_pImports->adjustColumn( i );

		emit finished();
		disconnect( this, 0, 0, 0 );
		deleteLater();
	}
}

ImportsExportsJob::ImportsExportsJob( QListViewItem *pItem, QListView *pImports, QListView *pExports ) :
	m_pItem( pItem ),
	m_pImports( pImports )
{
	pImports->clear();
	pExports->clear();

	ExportsJob *pJob = new ExportsJob( pItem->text( 2 ), pExports, &m_map );

	connect( pJob, SIGNAL( finished() ), this, SLOT( buildImports() ) );
}


void ImportsExportsJob::buildImports()
{
	if ( m_pItem->parent() != 0 )
	{
		ImportsJob *pJob = new ImportsJob( m_pItem->parent()->text( 2 ), m_pImports, &m_map );
		connect( pJob, SIGNAL( finished() ), this, SIGNAL( finished() ) );
	}
	else
		emit finished();
}
