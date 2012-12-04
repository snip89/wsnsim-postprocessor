#ifndef STATICCOREUTILS_CPP
#define STATICCOREUTILS_CPP

#include "staticcoreutils.h"

void StaticCoreUtils::setApplicationInfo()
{
    QCoreApplication::setOrganizationName("wsnsim");
    QCoreApplication::setApplicationName("logsvisualizer");
}

IHostRealTimeSettings *StaticCoreUtils::getHostRealTimeSettings()
{
    return new HostRealTimeSettings();
}

#endif // STATICCOREUTILS_CPP
