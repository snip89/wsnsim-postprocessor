#include "staticvisualizationtools.h"

QString StaticVisualizationTools::updateValue(int eventID,
                                              int argumentID,
                                              QVariant value,
                                              LogDataType type,
                                              QList<Format *> formats)
{
    QString result;

    if(formats.size() == 0)
    {
        result = updateValue(value, type);
    }
    else
    {
        foreach(Format *format, formats)
        {
            if(format->argument["eventID"].toInt() == eventID &&
                    format->argument["argumentID"].toInt() == argumentID &&
                    type == BYTE_ARRAY_TYPE)
            {
                result = applyFormat(value, format);
            }
            else
            {
                result = updateValue(value, type);
            }
        }
    }

    return result;
}

QString StaticVisualizationTools::applyFormat(QVariant value, Format *format)
{
    QString result;

    QFileInfo formatFileInfo(format->fileName);
    QDir::setCurrent(formatFileInfo.absoluteDir().absolutePath());

    QString errorString;

    StaticLuaLoader::loadFile(format->luaInfo["file"], errorString);

    //result += "Format: " + format->formatInfo["name"] + "; ";

    QList< QPair<QString, QVariant> > values = StaticLuaLoader::exec(format->luaInfo["file"], value.value<QByteArray>(), errorString);
    if(!errorString.isNull())
    {
        result += errorString + ": ";

        if(value.type() == QVariant::ByteArray)
        {
            QString hexed_string = "";
            foreach(quint8 nextHex, value.value<QByteArray>())
            {
                QString hexed = QString::number(nextHex, 16);
                hexed = hexed.toUpper();
                if(hexed.size() == 1)
                    hexed.insert(0, '0');

                hexed_string += hexed + " ";
            }

            hexed_string.chop(1);

            result += hexed_string;
        }

        return result;
    }

    for(int i = 0; i < values.size(); i++)
    {
        result += values[i].first + ": ";

        if(values[i].second.type() == QVariant::ByteArray)
        {
            QString hexed_string = "";
            foreach(quint8 nextHex, values[i].second.value<QByteArray>())
            {
                QString hexed = QString::number(nextHex, 16);
                hexed = hexed.toUpper();
                if(hexed.size() == 1)
                    hexed.insert(0, '0');

                hexed_string += hexed + " ";
            }

            hexed_string.chop(1);

            result += hexed_string;

            result += "; ";
        }
        else
        {
            result += values[i].second.toString();
            result += "; ";
        }
    }

    result.chop(2);

    return result;

    /*QString result;

    qint64 size = value.value<QByteArray>().size();
    int i = 0;

    foreach(FieldInfo field, format->fieldsInfo)
    {
        result += field["name"] + ": ";

        if(LogDataTypes::toLogDataType(field["type"]) == UINT8_TYPE)
        {
            if(i + sizeof(quint8) > size)
            {
                result += QObject::tr("bad size; ");
                i += sizeof(quint8);
            }
            else
            {

                quint8 fieldValue;

                StaticRecordsReader::translateArg(value.value<QByteArray>().data(),
                                                  size,
                                                  i,
                                                  fieldValue);

                i += sizeof(quint8);

                result += QString::number(fieldValue) + "; ";

            }
        }
        else if(LogDataTypes::toLogDataType(field["type"]) == UINT16_TYPE)
        {
            if(i + sizeof(quint16) > size)
            {
                result += QObject::tr("bad size; ");
                i += sizeof(quint16);
            }
            else
            {

                quint16 fieldValue;

                StaticRecordsReader::translateArg(value.value<QByteArray>().data(),
                                                  size,
                                                  i,
                                                  fieldValue);

                i += sizeof(quint16);

                result += QString::number(fieldValue) + "; ";

            }
        }
        else if(LogDataTypes::toLogDataType(field["type"]) == UINT32_TYPE)
        {
            if(i + sizeof(quint32) > size)
            {
                result += QObject::tr("bad size; ");
                i += sizeof(quint32);
            }
            else
            {

                quint32 fieldValue;

                StaticRecordsReader::translateArg(value.value<QByteArray>().data(),
                                                  size,
                                                  i,
                                                  fieldValue);

                i += sizeof(quint32);

                result += QString::number(fieldValue) + "; ";

            }
        }
        else if(LogDataTypes::toLogDataType(field["type"]) == UINT64_TYPE)
        {
            if(i + sizeof(quint64) > size)
            {
                result += QObject::tr("bad size; ");
                i += sizeof(quint64);
            }
            else
            {

                quint64 fieldValue;

                StaticRecordsReader::translateArg(value.value<QByteArray>().data(),
                                                  size,
                                                  i,
                                                  fieldValue);

                i += sizeof(quint64);

                result += QString::number(fieldValue) + "; ";

            }
        }
        else if(LogDataTypes::toLogDataType(field["type"]) == INT8_TYPE)
        {
            if(i + sizeof(qint8) > size)
            {
                result += QObject::tr("bad size; ");
                i += sizeof(qint8);
            }
            else
            {

                qint8 fieldValue;

                StaticRecordsReader::translateArg(value.value<QByteArray>().data(),
                                                  size,
                                                  i,
                                                  fieldValue);

                i += sizeof(qint8);

                result += QString::number(fieldValue) + "; ";

                }
        }
        else if(LogDataTypes::toLogDataType(field["type"]) == INT32_TYPE)
        {
            if(i + sizeof(qint32) > size)
            {
                result += QObject::tr("bad size; ");
                i += sizeof(qint32);
            }
            else
            {

                qint32 fieldValue;

                StaticRecordsReader::translateArg(value.value<QByteArray>().data(),
                                                  size,
                                                  i,
                                                  fieldValue);

                i += sizeof(qint32);

                result += QString::number(fieldValue) + "; ";

            }
        }
        else if(LogDataTypes::toLogDataType(field["type"]) == DOUBLE_TYPE)
        {
            if(i + sizeof(double) > size)
            {
                result += QObject::tr("bad size; ");
                i += sizeof(double);
            }
            else
            {

                double fieldValue;

                StaticRecordsReader::translateArg(value.value<QByteArray>().data(),
                                                  size,
                                                  i,
                                                  fieldValue);

                i += sizeof(double);

                result += QString::number(fieldValue) + "; ";

            }
        }
        else if(LogDataTypes::toLogDataType(field["type"]) == BOOL_TYPE)
        {
            if(i + sizeof(bool) > size)
            {
                result += QObject::tr("bad size; ");
                i += sizeof(bool);
            }
            else
            {

                bool fieldValue;

                StaticRecordsReader::translateArg(value.value<QByteArray>().data(),
                                                  size,
                                                  i,
                                                  fieldValue);

                i += sizeof(bool);

                if(fieldValue == true)
                    result += "true; ";
                else
                    result += "false; ";

            }
        }
        else if(LogDataTypes::toLogDataType(field["type"]) == BYTE_ARRAY_TYPE)
        {
            int formatSize = formatLength(format);
            int valueLength = size - formatSize;

            if(i + valueLength > size)
            {
                result += QObject::tr("bad size; ");
                i += valueLength;
            }
            else
            {
                for(int k = i; k < i + valueLength; k ++)
                {
                    QString hexed_string = "";

                    quint8 nextHex = value.value<QByteArray>()[k];

                    QString hexed = QString::number(nextHex, 16);
                    hexed = hexed.toUpper();
                    if(hexed.size() == 1)
                        hexed.insert(0, '0');

                    hexed_string += hexed + " ";

                    result += hexed_string;
                }

                i += valueLength;
            }
        }
        else if(LogDataTypes::toLogDataType(field["type"]) == STRING_TYPE)
        {
            int formatSize = formatLength(format);
            int valueLength = size - formatSize;

            if(i + valueLength > size)
            {
                result += QObject::tr("bad size; ");
                i += valueLength;
            }
            else
            {
                for(int k = i; k < i + valueLength; k ++)
                {
                    result += value.value<QByteArray>()[k];
                }

                i += valueLength;
            }
        }
    }

    if(i < size)
    {
        result += QObject::tr("unrecognized") + ": ";

        for(int k = i; k < size; k ++)
        {
            QString hexed_string = "";

            quint8 nextHex = value.value<QByteArray>()[k];

            QString hexed = QString::number(nextHex, 16);
            hexed = hexed.toUpper();
            if(hexed.size() == 1)
                hexed.insert(0, '0');

            hexed_string += hexed + " ";

            result += hexed_string;
        }
    }

    return result;*/
}

QString StaticVisualizationTools::updateValue(QVariant value, LogDataType type)
{
    QString result;

    if(type == BYTE_ARRAY_TYPE)
    {
        QString hexed_string = "";
        foreach(quint8 nextHex, value.value<QByteArray>())
        {
            QString hexed = QString::number(nextHex, 16);
            hexed = hexed.toUpper();
            if(hexed.size() == 1)
                hexed.insert(0, '0');

            hexed_string += hexed + " ";
        }

        hexed_string.chop(1);

        result = hexed_string;
    }
    else
    {
        result = value.toString();
    }

    return result;
}

int StaticVisualizationTools::formatLength(Format *format)
{
    /*int result = 0;

    foreach(FieldInfo field, format->fieldsInfo)
    {
        if(LogDataTypes::toLogDataType(field["type"]) == UINT8_TYPE)
            result += sizeof(quint8);

        else if(LogDataTypes::toLogDataType(field["type"]) == UINT16_TYPE)
            result += sizeof(quint16);

        else if(LogDataTypes::toLogDataType(field["type"]) == UINT32_TYPE)
            result += sizeof(quint32);

        else if(LogDataTypes::toLogDataType(field["type"]) == UINT64_TYPE)
            result += sizeof(quint64);

        else if(LogDataTypes::toLogDataType(field["type"]) == INT8_TYPE)
            result += sizeof(qint8);

        else if(LogDataTypes::toLogDataType(field["type"]) == INT32_TYPE)
            result += sizeof(qint32);

        else if(LogDataTypes::toLogDataType(field["type"]) == DOUBLE_TYPE)
            result += sizeof(double);

        else if(LogDataTypes::toLogDataType(field["type"]) == BOOL_TYPE)
            result += sizeof(bool);
    }

    return result;*/
}

QStringList StaticVisualizationTools::argumentsNames(SimpleEventInfo *info, int infoSize)
{
    QStringList result;

    for(int i = 0; i < infoSize; i++)
    {
        int argsCount = info[i].argsCount;

        SimpleArgInfo *argsInfo = info[i].argsInfo;
        for(int j = 0; j < argsCount; j++)
        {
            if(!result.contains(*argsInfo[j].name))
                result.append(*argsInfo[j].name);
        }
    }

    //result.sort();

    result.insert(0, "event");
    result.insert(0, "time");

    return result;
}
