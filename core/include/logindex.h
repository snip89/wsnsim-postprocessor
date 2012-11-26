/*
 * logindex.h
 *
 *  Created on: May 7, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#ifndef LOGINDEX_H_
#define LOGINDEX_H_

#include <QFile>
#include <malloc.h>
#include <QDebug>
#include <QErrorMessage>
#include <QProgressDialog>
#include <QCoreApplication>

#include "simpleeventinfo.h"
#include "staticlogreader.h"
#include "staticrecordsreader.h"
#include "staticfromlogselector.h"

struct Index
{
    qint64 filePos;
    quint64 time;
};

class LogIndex
{
public:
    friend class Log;
    friend class StaticLogFilter;
    friend class FiltrationWidget;
    friend class StaticFromLogSelector;

    LogIndex(QFile *logFile, qint64 bSize, qint64 mSize, SimpleEventInfo *info);
    bool generate();
    Index at(qint64 i);
    qint64 size();
    virtual ~LogIndex();

private:
    QFile *file;

    qint64 blockSize;
    qint64 memorySize;

    qint64 logSize;

    SimpleEventInfo *eventsInfo;

    Index *index;
    qint64 indexSize;
    qint64 blockElementsCount;
//    qint64 currentBlockSize;
    qint64 filePos;

    QErrorMessage errorMessager;

    void append(qint64 pos, quint64 time);
    bool generateFromMemory(char *memory, qint64 memorySize, qint64 &logSize, QProgressDialog &dlg);
};

#endif /* LOGINDEX_H_ */
