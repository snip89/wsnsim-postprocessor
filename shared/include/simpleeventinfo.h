/*
 * simpleeventinfo.h
 *
 *  Created on: May 7, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#ifndef SIMPLEEVENTINFO_H_
#define SIMPLEEVENTINFO_H_

#include <QString>

#include "logdatatypes.h"

struct SimpleArgInfo
{
    LogDataType type;
    QString *name;
    int size;
};

struct SimpleEventInfo
{
    int size;
    int argsCount;
    QString *type;
    QString *group;
    SimpleArgInfo *argsInfo;
};

#endif /* SIMPLEEVENTINFO_H_ */
