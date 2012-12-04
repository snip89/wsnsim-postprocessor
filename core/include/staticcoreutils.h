#ifndef STATICCOREUTILS_H
#define STATICCOREUTILS_H

#include <QCoreApplication>
#include <QDir>
#include <QLibrary>
#include <QApplication>
#include <QPluginLoader>
#include <QErrorMessage>
#include <QDebug>

#include "ihostrealtimesettings.h"
#include "simpleeventinfo.h"
#include "hostrealtimesettings.h"

class StaticCoreUtils
{
public:
    static void setApplicationInfo();
    static IHostRealTimeSettings *getHostRealTimeSettings();
};

#endif // STATICCOREUTILS_H
