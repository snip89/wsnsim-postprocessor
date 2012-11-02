#include "logdatatypes.h"

LogDataType LogDataTypes::toLogDataType(QString type)
{
    if(type == "uint8")
        return UINT8_TYPE;

    else if(type == "uint16")
        return UINT16_TYPE;

    else if(type == "uint32")
        return UINT32_TYPE;

    else if(type == "uint64")
        return UINT64_TYPE;

    else if(type == "int32")
        return INT32_TYPE;

    else if(type == "bool")
        return BOOL_TYPE;

    else if(type == "double")
        return DOUBLE_TYPE;

    else if(type == "ByteArray")
        return BYTE_ARRAY_TYPE;

    else if(type == "string")
        return STRING_TYPE;

    return UNKNOWN_TYPE;
}
