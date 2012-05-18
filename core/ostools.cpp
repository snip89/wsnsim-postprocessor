/*
 * ostools.cpp
 *
 *  Created on: May 7, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#include "ostools.h"

qint64 OSTools::getTotalSystemMemory()
{
    qint64 result = UNSTABLE_DEFAULT_MEMORY;

// working fine for nix
#ifdef Q_WS_X11

    QFile meminfo("/proc/meminfo");
    meminfo.open(QFile::ReadOnly);

    QString nextLine = meminfo.readLine();

    while (nextLine != QString::null)
    {
        QTextStream in(&nextLine);
        QString paramName;

        in >> paramName;
        if (paramName == "MemTotal:")
        {
            in >> result;
            return result * 1024;
        }

        nextLine = meminfo.readLine();
    }

#endif

#ifdef Q_WS_WIN

// working fine but some win os's haven't systeminfo.exe

    QProcess sysinfo;
    int processState = sysinfo.execute("systeminfo.exe", QStringList());
    if(processState == -1 || processState == -2)
        return result;
    else
    {
        sysinfo.waitForFinished();

        QByteArray bsysinfo = sysinfo.readAll();
        QList<QByteArray> sbsysinfo = bsysinfo.split( '\r' );

        foreach (QByteArray thissysinfo, sbsysinfo)
        {
            QString str(thissysinfo);
            if (str.contains("Total Physical Memory:"))
            {
                str = str.remove("\nTotal Physical Memory:").remove("MB").remove(0xFF).remove(' ');
                result = str.toLongLong() * 1024 * 1024;
                return result;
            }
        }
    }

#endif

    return result;
}
