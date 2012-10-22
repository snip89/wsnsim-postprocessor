#ifndef STATICCOREUTILS_CPP
#define STATICCOREUTILS_CPP

#include "staticcoreutils.h"

void StaticCoreUtils::setApplicationInfo()
{
    QCoreApplication::setOrganizationName("wsnsim");
    QCoreApplication::setApplicationName("logsvisualizer");
}

IRealTimeSettings *StaticCoreUtils::getRealTimeSettings()
{
    QDir::setCurrent(QApplication::applicationDirPath());

    QLibrary globalSettings("./globalSettings");

    typedef IRealTimeSettings*(*getRealTimeSettings) ();
    getRealTimeSettings func = (getRealTimeSettings) globalSettings.resolve("getRealTimeSettings");

    return func();
}

#endif // STATICCOREUTILS_CPP
