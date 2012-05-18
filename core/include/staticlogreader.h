/*
 * staticlogreader.h
 *
 *  Created on: May 7, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#ifndef STATICLOGREADER_H_
#define STATICLOGREADER_H_

#include <QFile>
#include <QDebug>

class StaticLogReader
{
public:
    static qint64 readLogFile(QFile &logFile, char *mem, qint64 maxMemSize);
    static qint64 readLogFile(QFile &logFile, char *mem, qint64 sPos, qint64 maxMemSize);
};

#endif /* STATICLOGREADER_H_ */
