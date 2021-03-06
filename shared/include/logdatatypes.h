/*
 * logdatatypes.h
 *
 *  Created on: May 7, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#ifndef LOGDATATYPES_H_
#define LOGDATATYPES_H_

#include <QString>

enum LogDataType
{
    UNKNOWN_TYPE,
    FIVE_BYTES_TYPE,
    UINT8_TYPE,
    INT8_TYPE,
    UINT16_TYPE,
    UINT32_TYPE,
    UINT64_TYPE,
    INT32_TYPE,
    BOOL_TYPE,
    DOUBLE_TYPE,
    BYTE_ARRAY_TYPE,
    STRING_TYPE
};

class LogDataTypes
{
public:
    static LogDataType toLogDataType(QString type);
};

#endif /* LOGDATATYPES_H_ */
