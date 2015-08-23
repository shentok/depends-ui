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
#ifndef INITJOB_H
#define INITJOB_H

#include <QObject>

#include <QMap>
#include <QProcess>
#include <QTextStream>

/**
 * @author Bernhard Beschow <bbeschow (.at) cs.tu-berlin.de>
 */
class InitJob : public QObject
{
	Q_OBJECT
public:
	InitJob( const QString &file, QMap<QString, QString> *pMap );

signals:
	void finished();

protected slots:
	void readLineStdout();

protected:
	QMap<QString, QString> *m_pMap;
	QProcess m_proc;
	QTextStream m_stream;
};

#endif
