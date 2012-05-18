/*
 * abstractfilter.h
 *
 *  Created on: May 11, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#ifndef ABSTRACTFILTER_H_
#define ABSTRACTFILTER_H_

#include <QString>
#include <QVariant>

class AbstractFilter
{
public:
    virtual QString argName() = 0;
    virtual bool check(QVariant value) = 0;
    virtual ~AbstractFilter() { }
};


#endif /* ABSTRACTFILTER_H_ */
