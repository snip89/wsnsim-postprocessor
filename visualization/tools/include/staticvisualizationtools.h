#ifndef STATICVISUALIZATIONTOOLS_H
#define STATICVISUALIZATIONTOOLS_H

#include <QString>
#include <QVariant>
#include <QList>
#include <QStringList>

#include "logdatatypes.h"
#include "formatParams.h"
#include "simpleeventinfo.h"
#include "staticrecordsreader.h"

class StaticVisualizationTools
{
public:
    static QString updateValue(
            int eventID,
            int argumentID,
            QVariant value,
            LogDataType type,
            QList<Format *> formats);

    static QString updateValue(QVariant value, LogDataType type);

    static QString applyFormat(QVariant value, LogDataType type, Format *format);

    static int formatLength(Format *format);

    static QStringList argumentsNames(SimpleEventInfo *info, int infoSize);
};

#endif // STATICVISUALIZATIONTOOLS_H
