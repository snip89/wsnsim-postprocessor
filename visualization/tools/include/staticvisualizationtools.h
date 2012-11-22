#ifndef STATICVISUALIZATIONTOOLS_H
#define STATICVISUALIZATIONTOOLS_H

#include <QString>
#include <QVariant>
#include <QList>
#include <QStringList>
#include <QDir>
#include <QFileInfo>

#include "logdatatypes.h"
#include "formatParams.h"
#include "simpleeventinfo.h"
#include "staticrecordsreader.h"
#include "staticlualoader.h"

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

    static QString applyFormat(QVariant value, Format *format);

    static QStringList argumentsNames(SimpleEventInfo *info, int infoSize);
};

#endif // STATICVISUALIZATIONTOOLS_H
