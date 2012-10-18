/*
 * records.h
 *
 *  Created on: May 11, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#ifndef RECORDS_H_
#define RECORDS_H_

#include <QByteArray>
#include <QList>
#include <QVariant>
#include <QString>

struct Record
{
    QByteArray byteRecord;
    quint64 time;
    quint8 eventID;
    QList<QVariant> other;
};

struct RecordArgument
{
    QString *name;
    QVariant *value;
};

#endif /* RECORDS_H_ */
