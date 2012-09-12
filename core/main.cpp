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
#include <QPoint>
#include <QSize>
#include <QTranslator>
#include <QObject>
#include <QFile>
#include <QTextStream>

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
    if(!settings.contains(QObject::tr("Defaults/General/Core/Block_size")))
        settings.setValue(QObject::tr("Defaults/General/Core/Block_size"), 100);

    if(!settings.contains(QObject::tr("General/Core/Block_size")))
        settings.setValue(QObject::tr("General/Core/Block_size"), 100);

    if(!settings.contains(QObject::tr("Hidden/Gui/Main_window_pos")))
        settings.setValue(QObject::tr("Hidden/Gui/Main_window_pos"), QPoint(0, 0));

    if(!settings.contains(QObject::tr("Hidden/Gui/Main_window_size")))
        settings.setValue(QObject::tr("Hidden/Gui/Main_window_size"), QSize(640, 480));

    if(!settings.contains(QObject::tr("Hidden/Gui/Main_window_is_full_screen")))
        settings.setValue(QObject::tr("Hidden/Gui/Main_window_is_full_screen"), false);

//    qint64 memSize = OSTools::getTotalSystemMemory();
//
//    if(memSize != -1)
//        memSize = memSize * 10 / 100;

    if(!settings.contains(QObject::tr("Defaults/General/Core/Memory_usage")))
        settings.setValue(QObject::tr("Defaults/General/Core/Memory_usage"), 10);

    if(!settings.contains(QObject::tr("General/Core/Memory_usage")))
        settings.setValue(QObject::tr("General/Core/Memory_usage"), 10);
}

int main(int argc, char **argv) {
    QApplication a(argc, argv);

    setUpCodec();
    setApplicationInfo();

    QSettings settings;

    /*if(!settings.contains("SYSTEM/Localization/Language"))
        settings.setValue("SYSTEM/Localization/Language", "En");

    QString language = settings.value("SYSTEM/Localization/Language").value<QString>();

    if(settings.contains("SYSTEM/Localization/Clear_settings"))
    {
        if(settings.value("SYSTEM/Localization/Clear_settings").value<bool>())
        {
            settings.clear();
            settings.setValue("SYSTEM/Localization/Language", language);
        }
    }

    QTranslator translator1;
    QTranslator translator2;

    if(language == "Ru")
    {   
        qDebug() << translator1.load("qt_ru");
        qDebug() << translator2.load("translation");

        a.installTranslator(&translator1);
        a.installTranslator(&translator2);
    }*/

    setSettings(settings);

    MainWindow w;

    w.move(settings.value(QObject::tr("Hidden/Gui/Main_window_pos")).value<QPoint>());
    w.resize(settings.value(QObject::tr("Hidden/Gui/Main_window_size")).value<QSize>());
    w.setFullScreen(settings.value(QObject::tr("Hidden/Gui/Main_window_is_full_screen")).value<bool>());

    w.show();
    if (argc == 2)
        w.openProject(argv[1]);

    int result = a.exec();

    settings.setValue(QObject::tr("Hidden/Gui/Main_window_pos"), w.pos());

    bool isMainWindowFullScreen = w.isFullScreen();

    if(!isMainWindowFullScreen)
        settings.setValue(QObject::tr("Hidden/Gui/Main_window_size"), w.size());

    settings.setValue(QObject::tr("Hidden/Gui/Main_window_is_full_screen"), isMainWindowFullScreen);

    // TODO: here can be saved windows size, pos

//    settings.clear();

    return result;
}
