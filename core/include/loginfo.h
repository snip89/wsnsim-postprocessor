/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef LOGINFO_H
#define LOGINFO_H

#include <QString>
#include <QStringList>

#include "log.h"

struct LogInfo
{
    int id;
    Log *log;
    QString fileName;
    QStringList filtersInfo;
};

#endif // LOGINFO_H
