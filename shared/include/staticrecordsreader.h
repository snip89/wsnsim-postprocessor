/*
 * staticrecordsreader.h
 *
 *  Created on: May 7, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#ifndef STATICRECORDSREADER_H_
#define STATICRECORDSREADER_H_

#include <QString>
#include <QStringList>
#include <QList>
#include <QVariant>
#include <QChar>
#include <QByteArray>
#include <QTextStream>

#include <memory.h>

#include "simpleeventinfo.h"
#include "records.h"

class StaticRecordsReader
{
    friend class StaticVisualizationTools;
    friend class StaticLuaLoader;
public:
    static bool skipRecord(char *mem, qint64 memSize, qint64 pos, qint64 &skippedSize, quint64 &time, SimpleEventInfo *info);
    static bool readRecord(char *mem, qint64 memSize, qint64 pos, qint64 &readedSize, Record &record, SimpleEventInfo *info);
    static bool checkRecord(char *mem, qint64 memSize, qint64 pos, qint64 &readedSize, bool &success, QString argName, QVariant &argValue, quint64 &time, SimpleEventInfo *info);
private:
    static bool skipArguments(char *mem, qint64 memSize, qint64 pos, qint64 &skippedSize, SimpleEventInfo *info, quint8 eventID);

    template <typename T>
    static bool translateArg(char *mem, qint64 memSize, qint64 sPos, T &value)
    {
        if(sPos + sizeof(T) > memSize)
            return false;

        char *bytesValue = new char[sizeof(T)];
        copyArgInvert(mem, bytesValue, sPos, sizeof(T));
        memcpy(&value, bytesValue, sizeof(T));

        delete[] bytesValue;
        return true;
    }

    static bool translateArg(char *mem, qint64 memSize, qint64 sPos, char *argValue, quint8 argSize);
    static bool translateArg(char *mem, qint64 memSize, qint64 sPos, QString &value, quint16 argSize);

    template <typename T>
    static bool translateArgUnusualSize(char *mem, qint64 memSize, qint64 sPos, T &value, int size)
    {
        if(sPos + size > memSize)
            return false;

        char *bytesValue = new char[size];
        copyArgInvert(mem, bytesValue, sPos, size);
        memcpy(&value, bytesValue, size);

        delete[] bytesValue;
        return true;
    }

    template <typename T>
    static bool readArgument(char *mem, qint64 memSize, qint64 &pos, qint64 &readedSize, T &value)
    {
        if(!translateArg(mem, memSize, pos, value))
            return false;

        pos += sizeof(T);
        readedSize += sizeof(T);

        return true;
    }

    template <typename T>
    static bool readArgumentUnusualSize(char *mem, qint64 memSize, qint64 &pos, qint64 &readedSize, T &value, int size)
    {
        if(!translateArgUnusualSize(mem, memSize, pos, value, size))
            return false;

        pos += size;
        readedSize += size;

        return true;
    }

    static bool readArguments(char *mem, qint64 memSize, qint64 pos, qint64 &readedSize, Record &record, SimpleEventInfo *info, quint8 eventID);
    static bool checkArguments(char *mem, qint64 memSize, qint64 pos, qint64 &readedSize, bool &success, QString argName, QVariant &argValue, SimpleEventInfo *info, quint8 eventID);

    static void copyArgInvert(char *mem, char *bytesValue, qint64 sPos, qint16 size);
    static void copyArg(char* mem, char *bytesValue, qint64 sPos, qint16 size);
};

#endif /* STATICRECORDSREADER_H_ */
