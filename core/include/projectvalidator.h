#ifndef PROJECTVALIDATOR_H
#define PROJECTVALIDATOR_H

#include <QString>

#include "projectParams.h"
#include "logdatatypes.h"

class ProjectValidator
{
public:
    static void validate(ProjectParams projectParams, QString &errorString);
};

#endif // PROJECTVALIDATOR_H
