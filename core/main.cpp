/*
 * main.cpp
 *
 *  Created on: May 5, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#include <QApplication>
#include <QTextCodec>
#include <QCoreApplication>
#include <QSettings>

#include "mainwindow.h"
#include "ostools.h"

void setUpCodec()
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
}

void setApplicationInfo()
{
    QCoreApplication::setOrganizationName("wsnsim");
    QCoreApplication::setApplicationName("postprocessor");
}

void setSettings(QSettings &settings)
{
    if(!settings.contains("Defaults/General/Core/Block_size"))
        settings.setValue("Defaults/General/Core/Block_size", 100);

    if(!settings.contains("General/Core/Block_size"))
        settings.setValue("General/Core/Block_size", 100);

//    qint64 memSize = OSTools::getTotalSystemMemory();
//
//    if(memSize != -1)
//        memSize = memSize * 10 / 100;

    if(!settings.contains("Defaults/General/Core/Memory_usage"))
        settings.setValue("Defaults/General/Core/Memory_usage", 10);

    if(!settings.contains("General/Core/Memory_usage"))
        settings.setValue("General/Core/Memory_usage", 10);
}

int main(int argc, char **argv) {
    QApplication a(argc, argv);

    setUpCodec();
    setApplicationInfo();

    QSettings settings;
    setSettings(settings);

    MainWindow w;
    w.show();
    if (argc == 2)
        w.openProject(argv[1]);

    int result = a.exec();

    // TODO: here can be saved windows size, pos

    return result;
}
