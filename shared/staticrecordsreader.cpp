/*
 * staticrecordsreader.cpp
 *
 *  Created on: May 7, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#include "staticrecordsreader.h"

bool StaticRecordsReader::skipRecord(char *mem, qint64 memSize, qint64 pos, qint64 &skippedSize, quint64 &time, SimpleEventInfo *info)
{
    time = 0;

    if(!translateArg(mem, memSize, pos, time))
        return false;

    pos += sizeof(quint64);

    skippedSize += sizeof(quint64);

    quint8 eventID = 0;

    if(!translateArg(mem, memSize, pos, eventID))
        return false;

    pos += sizeof(quint8);

    skippedSize += sizeof(quint8);

    if(!skipArguments(mem, memSize, pos, skippedSize, info, eventID))
        return false;

    return true;
}

bool StaticRecordsReader::readRecord(char *mem, qint64 memSize, qint64 pos, qint64 &readedSize, Record &record, SimpleEventInfo *info)
{
    quint64 time = 0;
    if(!translateArg(mem, memSize, pos, time))
        return false;

    readedSize += sizeof(quint64);

    char *bytesValue = new char[sizeof(quint64)];

    copyArg(mem, bytesValue, pos, sizeof(quint64));
    pos += sizeof(quint64);
    record.byteRecord.append(bytesValue, sizeof(quint64));
    record.time = time;

    delete[] bytesValue;

    quint8 eventID = 0;
    if(!translateArg(mem, memSize, pos, eventID))
        return false;
    readedSize += sizeof(quint8);

    bytesValue = new char[sizeof(quint8)];

    copyArg(mem, bytesValue, pos, sizeof(quint8));
    pos += sizeof(quint8);
    record.byteRecord.append(bytesValue, sizeof(quint8));
    record.eventID = eventID;

    delete[] bytesValue;

    if(!readArguments(mem, memSize, pos, readedSize, record, info, eventID))
        return false;

    return true;
}

bool StaticRecordsReader::checkRecord(char *mem, qint64 memSize, qint64 pos, qint64 &readedSize, bool &success, QString argName, QVariant &argValue, quint64 &time, SimpleEventInfo *info)
{
    time = 0;
    success = false;

    if(!translateArg(mem, memSize, pos, time))
        return false;

    pos += sizeof(quint64);

    readedSize += sizeof(quint64);

    if(argName == "time")
    {
        argValue.setValue(time);
        success = true;
    }

    quint8 eventID = 0;

    if(!translateArg(mem, memSize, pos, eventID))
        return false;

    pos += sizeof(quint8);

    readedSize += sizeof(quint8);

    if(argName == "eventID")
    {
        argValue.setValue(eventID);
        success = true;
    }

    if(!checkArguments(mem, memSize, pos, readedSize, success, argName, argValue, info, eventID))
        return false;

    return true;
}

bool StaticRecordsReader::skipArguments(char *mem, qint64 memSize, qint64 pos, qint64 &skippedSize, SimpleEventInfo *info, quint8 eventID)
{
    int fullSize = info[eventID].size;

    if(fullSize != -1)
    {
        if(pos + fullSize > memSize)
            return false;

        skippedSize += fullSize;
        return true;
    }
    else
    {
        for(int i = 0; i < info[eventID].argsCount; i ++)
        {
            if(info[eventID].argsInfo[i].type == FIVE_BYTES_TYPE)
            {
                if(pos + 5 > memSize)
                    return false;

                pos += 5;
                skippedSize += 5;
            }
            else if(info[eventID].argsInfo[i].type == UINT8_TYPE)
            {
                if(qint64(pos + sizeof(quint8)) > memSize)
                    return false;

                pos += sizeof(quint8);
                skippedSize += sizeof(quint8);
            }
            else if(info[eventID].argsInfo[i].type == INT8_TYPE)
            {
                if(qint64(pos + sizeof(qint8)) > memSize)
                    return false;

                pos += sizeof(qint8);
                skippedSize += sizeof(qint8);
            }
            else if(info[eventID].argsInfo[i].type == UINT16_TYPE)
            {
                if(qint64(pos + sizeof(quint16)) > memSize)
                    return false;

                pos += sizeof(quint16);
                skippedSize += sizeof(quint16);
            }
            else if(info[eventID].argsInfo[i].type == UINT32_TYPE)
            {
                if(qint64(pos + sizeof(quint32)) > memSize)
                    return false;

                pos += sizeof(quint32);

                skippedSize += sizeof(quint32);
            }
            else if(info[eventID].argsInfo[i].type == UINT64_TYPE)
            {
                if(qint64(pos + sizeof(quint64)) > memSize)
                    return false;

                pos += sizeof(quint64);
                skippedSize += sizeof(quint64);
            }
            else if(info[eventID].argsInfo[i].type == INT32_TYPE)
            {
                if(qint64(pos + sizeof(qint32)) > memSize)
                    return false;

                pos += sizeof(qint32);
                skippedSize += sizeof(qint32);
            }
            else if(info[eventID].argsInfo[i].type == BOOL_TYPE)
            {
                if(qint64(pos + sizeof(bool)) > memSize)
                    return false;

                pos += sizeof(bool);
                skippedSize += sizeof(bool);
            }
            else if(info[eventID].argsInfo[i].type == DOUBLE_TYPE)
            {
                if(qint64(pos + sizeof(double)) > memSize)
                    return false;

                pos += sizeof(double);
                skippedSize += sizeof(double);
            }
            else if(info[eventID].argsInfo[i].type == BYTE_ARRAY_TYPE)
            {
                quint8 argSize = 0;

                if(!translateArg(mem, memSize, pos, argSize))
                    return false;

                pos += sizeof(quint8);
                skippedSize += sizeof(quint8);

                if(pos + argSize > memSize)
                    return false;

                pos += argSize;
                skippedSize += argSize;
            }
            else if(info[eventID].argsInfo[i].type == STRING_TYPE)
            {
                quint16 argSize = 0;

                if(!translateArg(mem, memSize, pos, argSize))
                    return false;

                pos += sizeof(quint16);
                skippedSize += sizeof(quint16);

                if(pos + argSize > memSize)
                    return false;

                pos += argSize;
                skippedSize += argSize;
            }
        }
    }

    return true;
}

bool StaticRecordsReader::readArguments(char *mem, qint64 memSize, qint64 pos, qint64 &readedSize, Record &record, SimpleEventInfo *info, quint8 eventID)
{
    for(int i = 0; i < info[eventID].argsCount; i ++)
    {
        if(info[eventID].argsInfo[i].type == FIVE_BYTES_TYPE)
        {
            quint64 value = 0;
            qint64 temp = pos;

            if(!readArgumentUnusualSize(mem, memSize, pos, readedSize, value, 5))
                return false;

            char *bytesValue = new char[5];

            copyArg(mem, bytesValue, temp, 5);

            record.byteRecord.append(bytesValue, 5);
            record.other.append(QVariant(value));

            delete[] bytesValue;
        }
        else if(info[eventID].argsInfo[i].type == UINT8_TYPE)
        {
            quint8 value = 0;
            qint64 temp = pos;

            if(!readArgument(mem, memSize, pos, readedSize, value))
                return false;

            char *bytesValue = new char[sizeof(quint8)];

            copyArg(mem, bytesValue, temp, sizeof(quint8));

            record.byteRecord.append(bytesValue, sizeof(quint8));
            record.other.append(QVariant(value));

            delete[] bytesValue;
        }
        else if(info[eventID].argsInfo[i].type == INT8_TYPE)
        {
            qint8 value = 0;
            qint64 temp = pos;

            if(!readArgument(mem, memSize, pos, readedSize, value))
                return false;

            char *bytesValue = new char[sizeof(qint8)];

            copyArg(mem, bytesValue, temp, sizeof(qint8));

            record.byteRecord.append(bytesValue, sizeof(qint8));
            record.other.append(QVariant(value));

            delete[] bytesValue;
        }
        else if(info[eventID].argsInfo[i].type == UINT16_TYPE)
        {
            quint16 value = 0;
            qint64 temp = pos;

            if(!readArgument(mem, memSize, pos, readedSize, value))
                return false;

            char *bytesValue = new char[sizeof(quint16)];

            copyArg(mem, bytesValue, temp, sizeof(quint16));

            record.byteRecord.append(bytesValue, sizeof(quint16));
            record.other.append(QVariant(value));

            delete[] bytesValue;
        }
        else if(info[eventID].argsInfo[i].type == UINT32_TYPE)
        {
            quint32 value = 0;
            qint64 temp = pos;

            if(!readArgument(mem, memSize, pos, readedSize, value))
                return false;

            char *bytesValue = new char[sizeof(quint32)];

            copyArg(mem, bytesValue, temp, sizeof(quint32));
            record.byteRecord.append(bytesValue, sizeof(quint32));
            record.other.append(QVariant(value));

            delete[] bytesValue;
        }
        else if(info[eventID].argsInfo[i].type == UINT64_TYPE)
        {
            quint64 value = 0;
            qint64 temp = pos;

            if(!readArgument(mem, memSize, pos, readedSize, value))
                return false;

            char *bytesValue = new char[sizeof(quint64)];

            copyArg(mem, bytesValue, temp, sizeof(quint64));
            record.byteRecord.append(bytesValue, sizeof(quint64));
            record.other.append(QVariant(value));

            delete[] bytesValue;
        }
        else if(info[eventID].argsInfo[i].type == INT32_TYPE)
        {
            qint32 value = 0;
            qint64 temp = pos;

            if(!readArgument(mem, memSize, pos, readedSize, value))
                return false;

            char *bytesValue = new char[sizeof(qint32)];

            copyArg(mem, bytesValue, temp, sizeof(qint32));
            record.byteRecord.append(bytesValue, sizeof(qint32));
            record.other.append(QVariant(value));

            delete[] bytesValue;
        }
        else if(info[eventID].argsInfo[i].type == BOOL_TYPE)
        {
            bool value = false;
            qint64 temp = pos;

            if(!readArgument(mem, memSize, pos, readedSize, value))
                return false;

            char *bytesValue = new char[sizeof(bool)];

            copyArg(mem, bytesValue, temp, sizeof(bool));
            record.byteRecord.append(bytesValue, sizeof(bool));
            record.other.append(QVariant(value));

            delete[] bytesValue;
        }
        else if(info[eventID].argsInfo[i].type == DOUBLE_TYPE)
        {
            double value = 0;
            qint64 temp = pos;

            if(!readArgument(mem, memSize, pos, readedSize, value))
                return false;

            char *bytesValue = new char[sizeof(double)];

            copyArg(mem, bytesValue, temp, sizeof(double));
            record.byteRecord.append(bytesValue, sizeof(double));
            record.other.append(QVariant(value));

            delete[] bytesValue;
        }
        else if(info[eventID].argsInfo[i].type == BYTE_ARRAY_TYPE)
        {
            quint8 argSize = 0;

            if(!readArgument(mem, memSize, pos, readedSize, argSize))
                return false;

            char *value = new char[argSize + 1];
            if(!translateArg(mem, memSize, pos, value, argSize))
                return false;

            pos += argSize;
            readedSize += argSize;

            QByteArray arr(value, argSize);

            record.byteRecord.append(value, argSize);
            record.other.append(QVariant(arr));

            delete[] value;
        }
        else if(info[eventID].argsInfo[i].type == STRING_TYPE)
        {
            quint16 argSize = 0;

            if(!readArgument(mem, memSize, pos, readedSize, argSize))
                return false;

            char *byteValue = new char[argSize];
            if(!translateArg(mem, memSize, pos, byteValue, argSize))
                return false;

            record.byteRecord.append(byteValue, argSize);

            delete[] byteValue;

            QString value;
            if(!translateArg(mem, memSize, pos, value, argSize))
                return false;

            pos += argSize;
            readedSize += argSize;

            record.other.append(QVariant(value));
        }
    }

    return true;
}

bool StaticRecordsReader::checkArguments(char *mem, qint64 memSize, qint64 pos, qint64 &readedSize, bool &success, QString argName, QVariant &argValue, SimpleEventInfo *info, quint8 eventID)
{
    for(int i = 0; i < info[eventID].argsCount; i ++)
    {
        if(argName != info[eventID].argsInfo[i].name)
        {
            if(info[eventID].argsInfo[i].type == FIVE_BYTES_TYPE)
            {
                if(pos + 5 > memSize)
                    return false;

                pos += 5;
                readedSize += 5;
            }
            else if(info[eventID].argsInfo[i].type == UINT8_TYPE)
            {
                if(qint64(pos + sizeof(quint8)) > memSize)
                    return false;

                pos += sizeof(quint8);
                readedSize += sizeof(quint8);
            }
            else if(info[eventID].argsInfo[i].type == INT8_TYPE)
            {
                if(qint64(pos + sizeof(qint8)) > memSize)
                    return false;

                pos += sizeof(qint8);
                readedSize += sizeof(qint8);
            }
            else if(info[eventID].argsInfo[i].type == UINT16_TYPE)
            {
                if(qint64(pos + sizeof(quint16)) > memSize)
                    return false;

                pos += sizeof(quint16);
                readedSize += sizeof(quint16);
            }
            else if(info[eventID].argsInfo[i].type == UINT32_TYPE)
            {
                if(qint64(pos + sizeof(quint32)) > memSize)
                    return false;

                pos += sizeof(quint32);

                readedSize += sizeof(quint32);
            }
            else if(info[eventID].argsInfo[i].type == UINT64_TYPE)
            {
                if(qint64(pos + sizeof(quint64)) > memSize)
                    return false;

                pos += sizeof(quint64);
                readedSize += sizeof(quint64);
            }
            else if(info[eventID].argsInfo[i].type == INT32_TYPE)
            {
                if(qint64(pos + sizeof(qint32)) > memSize)
                    return false;

                pos += sizeof(qint32);
                readedSize += sizeof(qint32);
            }
            else if(info[eventID].argsInfo[i].type == BOOL_TYPE)
            {
                if(qint64(pos + sizeof(bool)) > memSize)
                    return false;

                pos += sizeof(bool);
                readedSize += sizeof(bool);
            }
            else if(info[eventID].argsInfo[i].type == DOUBLE_TYPE)
            {
                if(qint64(pos + sizeof(double)) > memSize)
                    return false;

                pos += sizeof(double);
                readedSize += sizeof(double);
            }
            else if(info[eventID].argsInfo[i].type == BYTE_ARRAY_TYPE)
            {
                quint8 argSize = 0;

                if(!translateArg(mem, memSize, pos, argSize))
                    return false;

                pos += sizeof(quint8);
                readedSize += sizeof(quint8);

                if(pos + argSize > memSize)
                    return false;

                pos += argSize;
                readedSize += argSize;
            }
            else if(info[eventID].argsInfo[i].type == STRING_TYPE)
            {
                quint16 argSize = 0;

                if(!translateArg(mem, memSize, pos, argSize))
                    return false;

                pos += sizeof(quint16);
                readedSize += sizeof(quint16);

                if(pos + argSize > memSize)
                    return false;

                pos += argSize;
                readedSize += argSize;
            }
        }

        else
        {
            if(info[eventID].argsInfo[i].type == FIVE_BYTES_TYPE)
            {
                quint64 value = 0;

                if(!readArgumentUnusualSize(mem, memSize, pos, readedSize, value, 5))
                    return false;

                argValue = QVariant(value);

                success = true;
            }
            else if(info[eventID].argsInfo[i].type == UINT8_TYPE)
            {
                quint8 value = 0;

                if(!readArgument(mem, memSize, pos, readedSize, value))
                    return false;

                argValue = QVariant(value);

                success = true;
            }
            else if(info[eventID].argsInfo[i].type == INT8_TYPE)
            {
                qint8 value = 0;

                if(!readArgument(mem, memSize, pos, readedSize, value))
                    return false;

                argValue = QVariant(value);

                success = true;
            }
            else if(info[eventID].argsInfo[i].type == UINT16_TYPE)
            {
                quint16 value = 0;

                if(!readArgument(mem, memSize, pos, readedSize, value))
                    return false;

                argValue = QVariant(value);

                success = true;
            }
            else if(info[eventID].argsInfo[i].type == UINT32_TYPE)
            {
                quint32 value = 0;

                if(!readArgument(mem, memSize, pos, readedSize, value))
                    return false;

                argValue = QVariant(value);

                success = true;
            }
            else if(info[eventID].argsInfo[i].type == UINT64_TYPE)
            {
                quint64 value = 0;

                if(!readArgument(mem, memSize, pos, readedSize, value))
                    return false;

                argValue = QVariant(value);

                success = true;
            }
            else if(info[eventID].argsInfo[i].type == INT32_TYPE)
            {
                qint32 value = 0;

                if(!readArgument(mem, memSize, pos, readedSize, value))
                    return false;

                argValue = QVariant(value);

                success = true;
            }
            else if(info[eventID].argsInfo[i].type == BOOL_TYPE)
            {
                bool value = false;

                if(!readArgument(mem, memSize, pos, readedSize, value))
                    return false;

                argValue = QVariant(value);

                success = true;
            }
            else if(info[eventID].argsInfo[i].type == DOUBLE_TYPE)
            {
                double value = 0;

                if(!readArgument(mem, memSize, pos, readedSize, value))
                    return false;

                argValue = QVariant(value);

                success = true;
            }
            else if(info[eventID].argsInfo[i].type == BYTE_ARRAY_TYPE)
            {
                quint8 argSize = 0;

                if(!readArgument(mem, memSize, pos, readedSize, argSize))
                    return false;

                char *value = new char[argSize + 1];
                if(!translateArg(mem, memSize, pos, value, argSize))
                    return false;

                pos += argSize;
                readedSize += argSize;

                argValue = QVariant(value);

                success = true;
            }
            else if(info[eventID].argsInfo[i].type == STRING_TYPE)
            {
                quint16 argSize = 0;

                if(!readArgument(mem, memSize, pos, readedSize, argSize))
                    return false;

                QString value;
                if(!translateArg(mem, memSize, pos, value, argSize))
                    return false;

                pos += argSize;
                readedSize += argSize;

                argValue = QVariant(value);

                success = true;
            }
        }
    }

    return true;
}

/*template <typename T>
bool StaticRecordsReader::translateArg(char *mem, qint64 memSize, qint64 sPos, T &value)
{
    if(sPos + sizeof(T) > memSize)
        return false;

    char *bytesValue = new char[sizeof(T)];
    copyArgInvert(mem, bytesValue, sPos, sizeof(T));
    memcpy(&value, bytesValue, sizeof(T));

    delete[] bytesValue;
    return true;
}*/

bool StaticRecordsReader::translateArg(char *mem, qint64 memSize, qint64 sPos, char *argValue, quint8 argSize)
{
    if(sPos + argSize > memSize)
        return false;
    copyArg(mem, argValue, sPos, argSize);
    argValue[argSize] = '\0';
    return true;
}

bool StaticRecordsReader::translateArg(char *mem, qint64 memSize, qint64 sPos, QString &value, quint16 argSize)
{
    if(sPos + argSize > memSize)
        return false;

    char *bytesValue = new char[argSize + 1];
    copyArg(mem, bytesValue, sPos, argSize);
    bytesValue[argSize] = '\0';
    value = QString(bytesValue);

    delete[] bytesValue;
    return true;
}

void StaticRecordsReader::copyArgInvert(char *mem, char *bytesValue, qint64 sPos, qint16 size)
{
    for(int i = 0; i < size; i ++)
    {
        bytesValue[i] = mem[(sPos + size) - (i + 1)];
    }
}

void StaticRecordsReader::copyArg(char *mem, char *bytesValue, qint64 sPos, qint16 size)
{
    for(int i = 0; i < size; i ++)
    {
        bytesValue[i] = mem[sPos + i];
    }
}
