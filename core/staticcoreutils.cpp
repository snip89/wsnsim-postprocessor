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
    QDir::setCurrent(QApplication::applicationDirPath());

    QLibrary globalSettings("./globalSettings");
    if(!globalSettings.load())
    {
        QErrorMessage errorMessager;

        errorMessager.showMessage(QObject::tr("Error while loading globalSettings library"));
        errorMessager.exec();

        exit(EXIT_SUCCESS);
    }

    typedef IHostRealTimeSettings*(*getHostRealTimeSettings) ();
    getHostRealTimeSettings func = (getHostRealTimeSettings) globalSettings.resolve("getHostRealTimeSettings");

    return func();
}

#endif // STATICCOREUTILS_CPP
