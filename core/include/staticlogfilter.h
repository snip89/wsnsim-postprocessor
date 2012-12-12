/*
 * staticlogfilter.h
 *
 *  Created on: May 8, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#ifndef STATICLOGFILTER_H_
#define STATICLOGFILTER_H_

#include <QVariant>
#include <QString>
#include <QStringList>
#include <QErrorMessage>
#include <QProgressDialog>
#include <QCoreApplication>

#include "log.h"
#include "staticrecordsreader.h"
#include "records.h"
#include "abstractfilter.h"
#include "staticlogwriter.h"

class StaticLogFilter
{
public:

    // TODO: must take some argument like filter expression
    static bool useFilter(Log *currentLog, Log *newLog, AbstractFilter *filter);
    static bool findRecord(Log* currentLog, QList<AbstractFilter*> filters, quint64 &pos);
private:
    static void appendToBuffer(char *fromBuffer, quint64 fromBufferPos, char *toBuffer, quint64 &toBufferPos, quint64 size);
};

#endif /* STATICLOGFILTER_H_ */
