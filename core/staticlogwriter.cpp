/*
 * staticlogwriter.cpp
 *
 *  Created on: May 12, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#include "staticlogwriter.h"

void StaticLogWriter::writeLogFile(QFile &logFile, char *mem, qint64 memSize)
{
    logFile.write(mem, memSize);
    logFile.flush();
}
