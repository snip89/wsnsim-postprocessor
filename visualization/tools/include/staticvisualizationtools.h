#ifndef STATICVISUALIZATIONTOOLS_H
#define STATICVISUALIZATIONTOOLS_H

#include <QString>
#include <QVariant>
#include <QList>
#include <QStringList>

#include "logdatatypes.h"
#include "formatParams.h"
#include "simpleeventinfo.h"

class StaticVisualizationTools
{
public:
    static QString updateValue(
            int eventID,
            int argumentID,
            QVariant value,
            LogDataType type,
            QList<Format*> formats);

    static int formatLength(Format *format);

    static QStringList argumentsNames(SimpleEventInfo *info, int infoSize);
};

#endif // STATICVISUALIZATIONTOOLS_H
