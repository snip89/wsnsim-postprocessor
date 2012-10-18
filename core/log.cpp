/*
 * log.cpp
 *
 *  Created on: May 8, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#include "log.h"

Log::Log(QString fileName, qint64 bSize, qint64 mSize, SimpleEventInfo *info, bool temp)
{
    file = new QFile(fileName);

    blockSize = bSize;
    memorySize = mSize;

    logicPos = 0;

    eventsInfo = info;

    isLogLoaded = false;
    isLogActivated = false;

    isTempLog = temp;

    updateFileSize();
}

bool Log::load(bool generateIndex, bool createNew)
{
    if(!createNew)
    {
        if(!file->exists())
        {
            errorMessager.showMessage("Log file not found");
            return false;
        }
    }

    if(!file->open(QFile::ReadWrite))
    {
        errorMessager.showMessage("Can't open log file");
        return false;
    }

    index = new LogIndex(file, blockSize, memorySize, eventsInfo);

    if(generateIndex)
    {
        index->generate();
        qDebug() << "logSize in records = " << index->logSize;
    }

    isLogLoaded = true;

    return true;
}

void Log::toggleActivity(bool activityStatus)
{
    if(activityStatus)
    {
        if(!isLogActivated)
        {
            memory = new char[memorySize];
            isLogActivated = true;
        }
    }
    else
    {
        if(isLogActivated)
        {
            delete[] memory;
            isLogActivated = false;
        }
    }
}

qint64 Log::size()
{
    return index->logSize;
}

void Log::seek(qint64 pos)
{
    logicPos = pos;
}

qint64 Log::pos()
{
    return logicPos;
}

char *Log::read(qint16 count, qint64 &size)
{
    qint64 sPos = getRecordFilePos(logicPos);

    qint64 ePos = getRecordFilePos(logicPos + count);

    size = StaticLogReader::readLogFile(*file, memory, sPos, ePos - sPos);

    logicPos += count;

    return memory;
}

QString Log::fileName()
{
    return file->fileName();
}

Log::~Log()
{
    if(isLogLoaded)
        delete index;

    toggleActivity(false);

    if(isTempLog)
        file->remove();

    delete file;
}

qint64 Log::getRecordFilePos(qint64 record)
{
    // получить номер блока, в котором находится запись
    qint64 block = record / blockSize;

    // получить индекс этого блока
    Index blockIndex = index->at(block);

    qint64 reallyMemSize;

    if((block + 1) >= index->size() - 1)
    {
        reallyMemSize = StaticLogReader::readLogFile(*file, memory, blockIndex.filePos, fileSize - blockIndex.filePos);
    }
    else
    {
        // получить индекс следующего блока
        Index nextBlockIndex = index->at(block + 1);

        // прочитать этот блок
        reallyMemSize = StaticLogReader::readLogFile(*file, memory, blockIndex.filePos, nextBlockIndex.filePos - blockIndex.filePos);
    }

    // количество записей от начала блока до нужной записи
    qint64 blockRecordsCount;

    if(block == 0)
        blockRecordsCount = record;
    else
        blockRecordsCount = record - ((block - 1) * blockSize + blockSize);

    // позиция в блоке
    qint64 memoryPos = 0;

    if(blockRecordsCount > 0)
    {

        // пропустить это количество записей
        for(qint64 i = 0; i < blockRecordsCount; i ++)
        {
            qint64 skippedSize = 0;
            quint64 time = 0;

            if(!StaticRecordsReader::skipRecord(memory, reallyMemSize, memoryPos, skippedSize, time, eventsInfo))
            {
                errorMessager.showMessage(QObject::tr("Unexpected end of record"));
                return 0;
            }

            memoryPos += skippedSize;
        }

    }


    // позиция в файле указывающая на запись
    return (blockIndex.filePos + memoryPos);
}

void Log::updateFileSize()
{
    fileSize = file->size();
}

void Log::deleteFile()
{
    file->remove();
}
