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

#include <QRegExp>

InitJob::InitJob( const QString &file, QMap<QString, QString> *pMap ) :
	m_pMap( pMap ),
	m_proc(),
	m_stream( &m_proc )
{
	m_proc.setReadChannel( QProcess::StandardOutput );

	connect( &m_proc, SIGNAL(readyReadStandardOutput()), this, SLOT(readLineStdout()) );
	connect( &m_proc, SIGNAL(finished(int)), this, SLOT(readLineStdout()) );

	const QStringList args = QStringList() << file;

	m_proc.start( "ldd", args );
}


void InitJob::readLineStdout()
{
	while ( m_proc.canReadLine() || !m_stream.atEnd() )
	{
		const QString line = m_stream.readLine();

		const QRegExp re( "^\\s+(\\S+) => (\\S+) \\(0x[0-9a-f]+\\)" );
		if ( re.indexIn( line ) >= 0 )
		{
			(*m_pMap)[ re.cap( 1 ) ] = re.cap( 2 );
		}
	}

	if ( m_proc.state() == QProcess::NotRunning )
	{
		emit finished();
		disconnect( this, 0, 0, 0 );
		deleteLater();
	}
}
