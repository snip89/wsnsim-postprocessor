/*
 * staticlogfilter.cpp
 *
 *  Created on: May 8, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#include "staticlogfilter.h"

bool StaticLogFilter::useFilter(Log *currentLog, Log *newLog, AbstractFilter *filter)
{
//    Log *newLog = new Log("temp.bin", currentLog->blockSize, currentLog->memorySize, currentLog->eventsInfo);
    newLog->load(false, true);

    qint64 currentLogPos = currentLog->pos();
    currentLog->seek(0);

    // index parameters
    newLog->index->logSize = 0;
    newLog->index->blockElementsCount = 0;
    newLog->index->filePos = 0;

    // TODO: allocate half of memory size here
    char *newLogBuffer = new char[newLog->memorySize];
    quint64 newLogBufferPos = 0;
    quint64 newLogBufferSize = 0;

    bool breakFirst = false;

    QProgressDialog dlg(QObject::tr("Filtration..."), QObject::tr("Cancel"), 0, 100);
    dlg.setWindowModality(Qt::WindowModal);

    while(currentLog->pos() < currentLog->size())
    {
        qDebug() << (currentLog->pos() * 100) / currentLog->size();

        if(dlg.wasCanceled())
            return false;

        dlg.setValue((currentLog->pos() * 100) / currentLog->size());
        QCoreApplication::processEvents();

        if(breakFirst)
            break;

        qint64 blockSize = 0;
        char *blockBuffer;

        if((currentLog->size() - currentLog->pos()) < currentLog->blockSize)
            blockBuffer = currentLog->read(currentLog->size() - currentLog->pos(), blockSize);

        else
            blockBuffer = currentLog->read(currentLog->blockSize, blockSize);

        qint64 pos = 0;
        while(pos < blockSize)
        {
            qint64 readedSize = 0;
            bool success = false;
            QVariant argValue;
            quint64 time = 0;
            if(!StaticRecordsReader::checkRecord(blockBuffer, blockSize, pos, readedSize, success, filter->argName(), argValue, time, currentLog->eventsInfo))
            {
                QErrorMessage errorMessager;
                errorMessager.showMessage(QObject::tr("Unexpected end of record"));
                return false;
            }

            if(success == true)
            {
                if(filter->check(argValue))
                {
                    appendToBuffer(blockBuffer, pos, newLogBuffer, newLogBufferPos, readedSize);
                    newLogBufferSize += readedSize;

                    if(newLog->index->indexSize == 1)
                    {
                        newLog->index->blockElementsCount ++;
                        newLog->index->logSize ++;

                        newLog->index->append(newLog->index->filePos, time);
                        newLog->index->filePos += readedSize;
                    }
                    else
                    {
                        newLog->index->blockElementsCount ++;
                        newLog->index->logSize ++;
                        newLog->index->filePos += readedSize;

                        if(newLog->index->blockElementsCount == newLog->index->blockSize)
                        {
                            newLog->index->append(newLog->index->filePos, time);

                            newLog->index->blockElementsCount = 0;

                            StaticLogWriter::writeLogFile(*newLog->file, newLogBuffer, newLogBufferSize);

                            newLogBufferPos = 0;
                            newLogBufferSize = 0;
                        }
                    }
                }
                else
                {
//                    if(filter->argName() == "time")
//                    {
//                        pos += readedSize;
//                        breakFirst = true;
//                        break;
//                    }
                }
            }

            pos += readedSize;
        }
    }

    if(newLog->index->blockElementsCount != 0)
    {
        newLog->index->blockElementsCount = 0;

        StaticLogWriter::writeLogFile(*newLog->file, newLogBuffer, newLogBufferSize);

        newLogBufferPos = 0;
        newLogBufferSize = 0;
    }

    currentLog->seek(currentLogPos);
//    currentLog->file->seek(currentLogPos);

    delete[] newLogBuffer;

    newLog->updateFileSize();

    // TODO: must return new Log

    return true;
}

void StaticLogFilter::appendToBuffer(char *fromBuffer, quint64 fromBufferPos, char *toBuffer, quint64 &toBufferPos, quint64 size)
{
    for(quint64 i = fromBufferPos; i < fromBufferPos + size; i ++)
    {
        toBuffer[toBufferPos ++] = fromBuffer[i];
    }
}
