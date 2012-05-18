/*
 * staticlogwriter.h
 *
 *  Created on: May 12, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#ifndef STATICLOGWRITER_H_
#define STATICLOGWRITER_H_

#include <QFile>
#include <QDebug>

class StaticLogWriter
{
public:
    static void writeLogFile(QFile &logFile, char *mem, qint64 memSize);
};

#endif /* STATICLOGWRITER_H_ */
