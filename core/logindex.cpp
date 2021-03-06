/*
 * logindex.cpp
 *
 *  Created on: May 7, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#include "logindex.h"

LogIndex::LogIndex(QFile *logFile, qint64 bSize, qint64 mSize, SimpleEventInfo *info)
{
    file = logFile;

    blockSize = bSize;
    memorySize = mSize;

    eventsInfo = info;

    index = (Index*)malloc(sizeof(Index));

    indexSize = 1;
}

bool LogIndex::generate()
{
    QProgressDialog dlg(QObject::tr("Generating index..."), QObject::tr("Cancel"), 0, 100);
    dlg.setWindowModality(Qt::WindowModal);

    logSize = 0;
    blockElementsCount = 0;
//    currentBlockSize = 0;
    filePos = 0;

    if(file->size() <= memorySize)
    {
        QCoreApplication::processEvents();

        char *memory = new char[memorySize];

        qint64 reallyReadedSize = StaticLogReader::readLogFile(*file, memory, memorySize);

        if(!generateFromMemory(memory, reallyReadedSize, logSize, dlg))
            return false;

        qDebug() << logSize;

        delete[] memory;
    }
    else
    {
        while(!file->atEnd())
        {
            char *memory = new char[memorySize];

            qint64 reallyReadedSize = StaticLogReader::readLogFile(*file, memory, memorySize);

            if(!generateFromMemory(memory, reallyReadedSize, logSize, dlg))
                return false;

            qDebug() << logSize;

            file->seek(filePos);

            delete[] memory;

            QCoreApplication::processEvents();
        }
    }

    return true;
}

Index LogIndex::at(qint64 i)
{
    return index[i];
}

LogIndex::~LogIndex()
{
    // освободить память из под индекса
    delete[] index;
}

void LogIndex::append(qint64 pos, quint64 time)
{
    // добавить элемент в индекс
    index[indexSize - 1].filePos = pos;
    index[indexSize - 1].time = time;

    // увеличен размер индекса
    indexSize ++;

    // выделить память под следующий элемент
    index = (Index*)realloc(index, sizeof(Index)*indexSize);
}

qint64 LogIndex::size()
{
    return indexSize;
}

bool LogIndex::generateFromMemory(char *memory, qint64 memorySize, qint64 &logSize, QProgressDialog &dlg)
{
    // текущая позиция в буфере
    qint64 pos = 0;

    // если в индексе еще ничего нет
    if(indexSize == 1)
    {
        qint64 skippedSize = 0;
        quint64 time = 0;

        // попытка пропустить запись
        StaticRecordsReader::skipRecord(memory, memorySize, pos, skippedSize, time, eventsInfo);

        // увеличить размер формируемого блока
//        currentBlockSize += skippedSize;

        // увеличить число записей в текущем блоке
        blockElementsCount ++;

        // общее число записей
        logSize ++;

        // добавить в индекс новый элемент
        append(filePos, time);

        // увеличить позиции в файле и буфере
        filePos += skippedSize;
        pos += skippedSize;
    }

    // пока не закончился буфер
    while(pos < memorySize)
    {
        if(dlg.wasCanceled())
            return false;

        //qDebug() << ((indexSize * memorySize + pos) * 100) / file->size();
        //qDebug() << memorySize;

        dlg.setValue((filePos * 100) / file->size());

        qint64 skippedSize = 0;
        quint64 time = 0;

        // попытка пропустить запись
        if(!StaticRecordsReader::skipRecord(memory, memorySize, pos, skippedSize, time, eventsInfo))
            return true;

        // увеличить размер формируемого блока
//        currentBlockSize += skippedSize;

        // увеличить число записей в текущем блоке
        blockElementsCount ++;

        // общее число записей
        logSize ++;

        // увеличить позиции в файле и буфере
        filePos += skippedSize;
        pos += skippedSize;

        // если блок заполнен полностью
        if(blockElementsCount == blockSize)
        {
            // добавить в индекс новый элемент
            append(filePos, time);

            // сбросить счетчик блоков
            blockElementsCount = 0;
        }
    }

    return true;
}
