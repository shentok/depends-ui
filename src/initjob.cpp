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
#include "initjob.h"

#include <qregexp.h>

InitJob::InitJob( const QString &file, QMap<QString, QString> *pMap ) :
	m_pMap( pMap )
{
	connect( &m_proc, SIGNAL( readyReadStdout() ), this, SLOT( readLineStdout() ) );
	connect( &m_proc, SIGNAL( processExited() ), this, SLOT( readLineStdout() ) );

	m_proc.addArgument( "ldd" );
	m_proc.addArgument( file );
	m_proc.start();
}


void InitJob::readLineStdout()
{
	while ( m_proc.canReadLineStdout() )
	{
		QString line = m_proc.readLineStdout();

		QRegExp re( "^\t(\\S+) => (\\S+) \\(0x[0-9a-f]{8}\\)" );
		if ( re.search( line ) >= 0 )
			(*m_pMap)[ re.cap( 1 ) ] = re.cap( 2 );
	}

	if ( ! m_proc.isRunning() )
	{
		emit finished();
		disconnect( this, 0, 0, 0 );
		deleteLater();
	}
}
