/*
 * filter.cpp
 *
 *  Created on: May 11, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#include "filter.h"

Filter::Filter(QString name, BooleanOperators op, QVariant cmpValue)
{
    argumentName = name;
    compareValue = cmpValue;
    booleanOperator = op;
}

QString Filter::argName()
{
    return argumentName;
}

bool Filter::check(QVariant argValue)
{
    switch (booleanOperator)
    {
    case EQ:
        if(argValue == compareValue)
            return true;
        break;
    case NE:
        if(argValue != compareValue)
            return true;
        break;
    case LT:
        if(argValue.type() == QVariant::Double)
        {
            if(argValue.value<double>() < compareValue.value<double>())
                return true;
        }
        else if(argValue.type() == QVariant::Int)
        {
            if(argValue.value<int>() < compareValue.value<int>())
                return true;
        }
        else if(argValue.type() == QVariant::UInt)
        {
            if(argValue.value<unsigned int>() < compareValue.value<unsigned int>())
                return true;
        }
        else if(argValue.type() == QVariant::ULongLong)
        {
            if(argValue.value<unsigned long long>() < compareValue.value<unsigned long long>())
                return true;
        }

        break;
    case LE:
        if(argValue.type() == QVariant::Double)
        {
            if(argValue.value<double>() <= compareValue.value<double>())
                return true;
        }
        else if(argValue.type() == QVariant::Int)
        {
            if(argValue.value<int>() <= compareValue.value<int>())
                return true;
        }
        else if(argValue.type() == QVariant::UInt)
        {
            if(argValue.value<unsigned int>() <= compareValue.value<unsigned int>())
                return true;
        }
        else if(argValue.type() == QVariant::ULongLong)
        {
            if(argValue.value<unsigned long long>() <= compareValue.value<unsigned long long>())
                return true;
        }

        break;
    case GT:
        if(argValue.type() == QVariant::Double)
        {
            if(argValue.value<double>() > compareValue.value<double>())
                return true;
        }
        else if(argValue.type() == QVariant::Int)
        {
            if(argValue.value<int>() > compareValue.value<int>())
                return true;
        }
        else if(argValue.type() == QVariant::UInt)
        {
            if(argValue.value<unsigned int>() > compareValue.value<unsigned int>())
                return true;
        }
        else if(argValue.type() == QVariant::ULongLong)
        {
            if(argValue.value<unsigned long long>() > compareValue.value<unsigned long long>())
                return true;
        }

        break;
    case GE:
        if(argValue.type() == QVariant::Double)
        {
            if(argValue.value<double>() >= compareValue.value<double>())
                return true;
        }
        else if(argValue.type() == QVariant::Int)
        {
            if(argValue.value<int>() >= compareValue.value<int>())
                return true;
        }
        else if(argValue.type() == QVariant::UInt)
        {
            if(argValue.value<unsigned int>() >= compareValue.value<unsigned int>())
                return true;
        }
        else if(argValue.type() == QVariant::ULongLong)
        {
            if(argValue.value<unsigned long long>() >= compareValue.value<unsigned long long>())
                return true;
        }

        break;
    }

    return false;
}

Filter::~Filter()
{
    // TODO Auto-generated destructor stub
}

