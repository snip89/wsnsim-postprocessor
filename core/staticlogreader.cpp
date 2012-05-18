/*
 * staticlogreader.cpp
 *
 *  Created on: May 7, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#include "staticlogreader.h"

qint64 StaticLogReader::readLogFile(QFile &logFile, char *mem, qint64 maxMemSize)
{
    // прочитать все данные из файла
    return logFile.read(mem, maxMemSize);
}

qint64 StaticLogReader::readLogFile(QFile &logFile, char *mem, qint64 sPos, qint64 maxMemSize)
{
    // установить нужную позицию в файле
    logFile.seek(sPos);

    // прочитать данные из файла
    return logFile.read(mem, maxMemSize);
}
