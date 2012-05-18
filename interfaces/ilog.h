/*
 * ilog.h
 *
 *  Created on: May 8, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#ifndef ILOG_H_
#define ILOG_H_

class ILog
{
public:
    virtual qint64 size() = 0;
    virtual void seek(qint64 pos) = 0;
    virtual qint64 pos() = 0;
    virtual char *read(qint16 count, qint64 &size) = 0;
    virtual ~ILog() { }
};

#endif /* ILOG_H_ */
