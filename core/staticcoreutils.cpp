#ifndef STATICCOREUTILS_CPP
#define STATICCOREUTILS_CPP

#include "staticcoreutils.h"

void StaticCoreUtils::setPrivateApplicationInfo()
{
    QCoreApplication::setOrganizationName("LogsVisualizer");
    QCoreApplication::setApplicationName("Private");
}

void StaticCoreUtils::setPublicApplicationInfo()
{
    QCoreApplication::setOrganizationName("LogsVisualizer");
    QCoreApplication::setApplicationName("Public");
}

#endif // STATICCOREUTILS_CPP
