#ifndef STATICCOREUTILS_H
#define STATICCOREUTILS_H

#include <QCoreApplication>
#include <QDir>
#include <QLibrary>
#include <QApplication>
#include <QPluginLoader>
#include <QDebug>

#include "irealtimesettings.h"

class StaticCoreUtils
{
public:
    static void setApplicationInfo();
    static IRealTimeSettings *getRealTimeSettings();
};

#endif // STATICCOREUTILS_H
