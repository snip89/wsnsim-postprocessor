/*
 * log.h
 *
 *  Created on: May 8, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#ifndef LOG_H_
#define LOG_H_

#include <QString>
#include <QFile>
#include <QDebug>
#include <QErrorMessage>

#include "ilog.h"
#include "simpleeventinfo.h"
#include "logindex.h"
#include "staticlogreader.h"
#include "staticrecordsreader.h"

class Log : public ILog
{
public:
    friend class StaticLogFilter;
    friend class FiltrationWidget;

    Log(QString fileName, qint64 bSize, qint64 mSize, SimpleEventInfo *info, bool temp);
    bool load(bool generateIndex, bool createNew);
    void toggleActivity(bool activityStatus);

    /*virtual*/ qint64 size();
    /*virtual*/ void seek(qint64 pos);
    /*virtual*/ qint64 pos();
    /*virtual*/ char *read(qint16 count, qint64 &size);

    virtual ~Log();

private:
    QFile *file;

    qint64 blockSize;
    qint64 memorySize;
    qint64 fileSize;
    qint64 logicPos;

    bool isLogLoaded;
    bool isLogActivated;
    bool isTempLog;

    SimpleEventInfo *eventsInfo;
    LogIndex *index;
    char *memory;

    QErrorMessage errorMessager;

    qint64 getRecordFilePos(qint64 record);
    void updateFileSize();
    void deleteFile();
};

#endif /* LOG_H_ */
