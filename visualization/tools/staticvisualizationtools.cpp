#include "staticvisualizationtools.h"

QString StaticVisualizationTools::updateValue(
        int eventID,
        int argumentID,
        QVariant value,
        LogDataType type,
        QList<Format*> formats)
{
    QString result;

    if(formats.size() == 0)
    {
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
    }
    else
    {
        foreach(Format *format, formats)
        {
            if(LogDataTypes::toLogDataType(format->formatInfo["argumentType"]) == type
                    && format->argument["eventID"].toInt() == eventID
                    && format->argument["argumentID"].toInt() == argumentID)
            {
                if(type == BYTE_ARRAY_TYPE)
                {
                    int length = 0;

                    if((format->formatInfo["unsizedField"].toInt() == 1))
                    {
                        length = formatLength(format);
                        length += value.value<QByteArray>().size() - length;
                    }
                    else
                    {
                        length = formatLength(format);
                    }

                    if(length != value.value<QByteArray>().size())
                    {
                        result += "Bad format(" + format->formatInfo["name"] + ") array: ";

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
                    else
                    {
                        int iterator = 0;

                        foreach(FieldInfo field, format->fieldsInfo)
                        {
                            result += field["name"] + ": ";

                            int fieldSize = field["length"].toInt();

                            if(fieldSize == -1)
                                fieldSize = value.value<QByteArray>().size() - formatLength(format);

                            for(int i = 0; i < fieldSize; i ++)
                            {
                                QString hexed = QString::number((quint8)value.value<QByteArray>()[iterator], 16);
                                hexed = hexed.toUpper();
                                if(hexed.size() == 1)
                                    hexed.insert(0, '0');

                                result += hexed + " ";

                                iterator ++;
                            }
                        }
                    }
                }
                else
                {
                    result = value.toString();
                }
            }
            else if(type == BYTE_ARRAY_TYPE)
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
        }
    }

    return result;
}

int StaticVisualizationTools::formatLength(Format *format)
{
    int result = 0;

    if(format->formatInfo["unsizedField"].toInt() != 1)
    {
        foreach(FieldInfo field, format->fieldsInfo)
        {
            result += field["length"].toInt();
        }
    }
    else
    {
        foreach(FieldInfo field, format->fieldsInfo)
        {
            if(field["length"].toInt() != -1)
                result += field["length"].toInt();
        }
    }

    return result;
}
