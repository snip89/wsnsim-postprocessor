/*
 * ostools.h
 *
 *  Created on: May 7, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#ifndef OSTOOLS_H_
#define OSTOOLS_H_

#include <QtCore>

#define UNSTABLE_DEFAULT_MEMORY 100000000

// includes needed by unix
#ifdef Q_WS_X11

#include <QTextStream>
#include <QFile>

#endif

// includes needed by windows
#ifdef Q_WS_WIN

#include <QProcess>
#include <QTextStream>

#endif

class OSTools
{
public:
    static qint64 getTotalSystemMemory();
};

#endif /* OSTOOLS_H_ */
