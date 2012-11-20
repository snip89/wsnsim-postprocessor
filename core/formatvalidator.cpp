#include "formatvalidator.h"

void FormatValidator::validate(Format *format, QString &errorString)
{
    if(format->formatInfo["name"] == "")
    {
        errorString = QObject::tr("No format name");
        return;
    }

    if(format->formatInfo["argument"] == "")
    {
        errorString = QObject::tr("No format argument name");
        return;
    }

    if(format->luaInfo["file"] == "")
    {
        errorString = QObject::tr("No lua file in format");
        return;
    }

    /*if(format->fieldsInfo.size() == 0)
    {
        errorString = QObject::tr("No format fields");
        return;
    }

    int stringsNumber = 0;
    int byteArraysNumber = 0;

    foreach(FieldInfo info, format->fieldsInfo)
    {
        if(info["name"] == "")
        {
            errorString = QObject::tr("No field name");
            return;
        }

        if(info["type"] == "")
        {
            errorString = QObject::tr("No field type");
            return;
        }

        if(LogDataTypes::toLogDataType(info["type"]) == STRING_TYPE)
            stringsNumber ++;

        if(LogDataTypes::toLogDataType(info["type"]) == BYTE_ARRAY_TYPE)
            byteArraysNumber ++;

        if(LogDataTypes::toLogDataType(info["type"]) == UNKNOWN_TYPE)
        {
            errorString = QObject::tr("Unknown field type");
            return;
        }
    }

    if(stringsNumber > 1)
    {
        errorString = QObject::tr("Field with type string must be only one");
        return;
    }

    if(byteArraysNumber > 1)
    {
        errorString = QObject::tr("Field with type ByteArray must be only one");
        return;
    }

    if(stringsNumber == 1 && byteArraysNumber == 1)
    {
        errorString = QObject::tr("Must be field with type string only or ByteArray only");
        return;
    }*/
}
