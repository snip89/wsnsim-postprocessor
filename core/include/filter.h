/*
 * filter.h
 *
 *  Created on: May 11, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#ifndef FILTER_H_
#define FILTER_H_

#include <QString>
#include <QVariant>
#include <QErrorMessage>

#include "abstractfilter.h"
#include "booleanoperators.h"

class Filter : public AbstractFilter
{
public:
    friend class FiltrationWidget;

    Filter(QString name, BooleanOperators op, QVariant cmpValue);

    /*virtual*/ QString argName();
    /*virtual*/ bool check(QVariant argValue);

    virtual ~Filter();
private:
    QString argumentName;
    QVariant compareValue;
    BooleanOperators booleanOperator;
};

#endif /* FILTER_H_ */
