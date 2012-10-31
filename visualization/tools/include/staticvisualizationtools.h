#ifndef STATICVISUALIZATIONTOOLS_H
#define STATICVISUALIZATIONTOOLS_H

#include <QString>
#include <QVariant>
#include <QList>

#include "logdatatypes.h"
#include "formatParams.h"

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
};

#endif // STATICVISUALIZATIONTOOLS_H
