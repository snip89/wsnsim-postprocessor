#ifndef FORMATVALIDATOR_H
#define FORMATVALIDATOR_H

#include <QString>
#include <QObject>

#include "formatParams.h"
#include "logdatatypes.h"

class FormatValidator
{
public:
    static void validate(Format *format, QString &errorString);
};

#endif // FORMATVALIDATOR_H
