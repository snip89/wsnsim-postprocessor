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
#include <QStringList>
#include <QTextStream>

#include "mainwindow.h"
#include "ostools.h"
#include "staticcoreutils.h"

void setUpCodec()
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
}

void setSettings(QSettings &settings)
{
    if(!settings.contains("Defaults/General/Core/Block_size"))
        settings.setValue("Defaults/General/Core/Block_size", 100);

    if(!settings.contains("General/Core/Block_size"))
        settings.setValue("General/Core/Block_size", 100);

    if(!settings.contains("Hidden/Gui/Main_window_pos"))
        settings.setValue("Hidden/Gui/Main_window_pos", QPoint(0, 0));

    if(!settings.contains("Hidden/Gui/Main_window_size"))
        settings.setValue("Hidden/Gui/Main_window_size", QSize(640, 480));

    if(!settings.contains("Hidden/Gui/Main_window_is_full_screen"))
        settings.setValue("Hidden/Gui/Main_window_is_full_screen", false);

//    qint64 memSize = OSTools::getTotalSystemMemory();
//
//    if(memSize != -1)
//        memSize = memSize * 10 / 100;

    if(!settings.contains("Defaults/General/Core/Memory_usage"))
        settings.setValue("Defaults/General/Core/Memory_usage", 10);

    if(!settings.contains("General/Core/Memory_usage"))
        settings.setValue("General/Core/Memory_usage", 10);

    if(!settings.contains("Localization/Language"))
        settings.setValue("Localization/Language", "En");

    if(!settings.contains("Defaults/Localization/Language"))
        settings.setValue("Defaults/Localization/Language", "En");
}

void setPublicSettings(QSettings &settings)
{
    if(!settings.contains("Main/ClientsList"))
        settings.setValue("Main/ClientsList", QStringList());
    else
    {
        QStringList clients = settings.value("Main/ClientsList").value<QStringList>();

        foreach(QString client, clients)
        {
            if(!settings.contains("Client/" + client + "/IP"))
                settings.setValue("Client/" + client + "/IP", "127.0.0.1");

            if(!settings.contains("Client/" + client + "Port"))
                settings.setValue("Client/" + client + "/Port", "10000");
        }
    }
}

int main(int argc, char **argv) {
    QApplication a(argc, argv);

    StaticCoreUtils::setPublicApplicationInfo();
    QSettings publicSettings;
    setPublicSettings(publicSettings);

    setUpCodec();
    StaticCoreUtils::setPrivateApplicationInfo();

    QSettings settings;

    setSettings(settings);

    QString language = settings.value("Localization/Language").value<QString>();

    QTranslator translator_qt;
    QTranslator translator_postprocessor;

    if(language == "Ru")
    {
        translator_qt.load(":/localization/qt_ru");
        translator_postprocessor.load(":/localization/postprocessor_ru");

        a.installTranslator(&translator_qt);
        a.installTranslator(&translator_postprocessor);
    }

    else if(language == "En")
    {
        translator_qt.load(":/localization/qt_en");
        translator_postprocessor.load(":/localization/postprocessor_en");

        a.installTranslator(&translator_qt);
        a.installTranslator(&translator_postprocessor);
    }

    MainWindow w;

    w.move(settings.value("Hidden/Gui/Main_window_pos").value<QPoint>());
    w.resize(settings.value("Hidden/Gui/Main_window_size").value<QSize>());
    w.setFullScreen(settings.value("Hidden/Gui/Main_window_is_full_screen").value<bool>());

    w.show();
    if (argc == 2)
        w.openProject(argv[1]);

    int result = a.exec();

    settings.setValue("Hidden/Gui/Main_window_pos", w.pos());

    bool isMainWindowFullScreen = w.isFullScreen();

    if(!isMainWindowFullScreen)
        settings.setValue("Hidden/Gui/Main_window_size", w.size());

    settings.setValue("Hidden/Gui/Main_window_is_full_screen", isMainWindowFullScreen);

    // TODO: here can be saved windows size, pos

//    settings.clear();

    return result;
}
