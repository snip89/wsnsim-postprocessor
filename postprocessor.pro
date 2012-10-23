######################################################################
# Automatically generated by qmake (2.01a) Mon Oct 22 18:32:24 2012
######################################################################

QT += core gui xml network
INCLUDEPATH += ../libs/projectData
INCLUDEPATH += ../libs/globalSettings/realtime/interfaces
TARGET += logsVisualizer
TEMPLATE = app
TARGET = 
DEPENDPATH += . \
              core \
              interfaces \
              resources \
              shared \
              ui \
              core/include \
              resources/localization \
              shared/include \
              visualization/abstract \
              visualization/hexvisualization \
              visualization/tablevisualization \
              visualization/textvisualization \
              visualization/tools \
              visualization/abstract/include \
              visualization/hexvisualization/include \
              visualization/realtime/hexvisualization \
              visualization/realtime/textvisualization \
              visualization/tablevisualization/include \
              visualization/textvisualization/include \
              visualization/tools/include \
              visualization/realtime/hexvisualization/include \
              visualization/realtime/textvisualization/include
INCLUDEPATH += . \
               core/include \
               ui \
               interfaces \
               shared/include \
               visualization/textvisualization/include \
               visualization/abstract/include \
               visualization/tools/include \
               visualization/hexvisualization/include \
               visualization/tablevisualization/include \
               visualization/realtime/textvisualization/include \
               visualization/realtime/hexvisualization/include

# Input
HEADERS += interfaces/ilog.h \
           interfaces/iproject.h \
           interfaces/irealtimevisualization.h \
           interfaces/ivisualization.h \
           ui/ui_hexappearancecolorsandfontssettings.h \
           ui/ui_sniffercoresettings.h \
           ui/ui_snifferguisettings.h \
           ui/ui_tablevisualization.h \
           core/include/abstractfilter.h \
           core/include/booleanoperators.h \
           core/include/clientsettings.h \
           core/include/filter.h \
           core/include/filtrationwidget.h \
           core/include/generalcoresettings.h \
           core/include/generalguisettings.h \
           core/include/gotolinedialog.h \
           core/include/localizationsettings.h \
           core/include/log.h \
           core/include/logindex.h \
           core/include/loginfo.h \
           core/include/logselectdialog.h \
           core/include/mainsettings.h \
           core/include/mainwindow.h \
           core/include/openconnectiondialog.h \
           core/include/ostools.h \
           core/include/project.h \
           core/include/staticcoreutils.h \
           core/include/staticlogfilter.h \
           core/include/staticlogreader.h \
           core/include/staticlogwriter.h \
           shared/include/emptysettings.h \
           shared/include/logdatatypes.h \
           shared/include/records.h \
           shared/include/simpleeventinfo.h \
           shared/include/staticfromlogselector.h \
           shared/include/staticrecordsreader.h \
           visualization/abstract/include/abstractrealtimetextvisualization.h \
           visualization/abstract/include/abstracttablevisualization.h \
           visualization/abstract/include/abstracttextappearancecolorsandfontssettings.h \
           visualization/abstract/include/abstracttextguisettings.h \
           visualization/abstract/include/abstracttextvisualization.h \
           visualization/hexvisualization/include/hexappearancecolorsandfontssettings.h \
           visualization/hexvisualization/include/hexguisettings.h \
           visualization/hexvisualization/include/hexvisualization.h \
           visualization/tablevisualization/include/tableguisettings.h \
           visualization/tablevisualization/include/tablevisualization.h \
           visualization/textvisualization/include/textappearancecolorsandfontssettings.h \
           visualization/textvisualization/include/textguisettings.h \
           visualization/textvisualization/include/textvisualization.h \
           visualization/tools/include/tableviewer.h \
           visualization/tools/include/viewer.h \
           visualization/realtime/hexvisualization/include/realtimehexvisualization.h \
           visualization/realtime/textvisualization/include/realtimetextvisualization.h
FORMS += ui/abstractrealtimetextvisualization.ui \
         ui/abstracttablevisualization.ui \
         ui/abstracttextappearancecolorsandfontssettings.ui \
         ui/abstracttextguisettings.ui \
         ui/abstracttextvisualization.ui \
         ui/clientsettings.ui \
         ui/emptysettings.ui \
         ui/filtrationwidget.ui \
         ui/generalcoresettings.ui \
         ui/generalguisettings.ui \
         ui/gotolinedialog.ui \
         ui/localizationsettings.ui \
         ui/logselectdialog.ui \
         ui/mainsettings.ui \
         ui/mainwindow.ui \
         ui/openconnectiondialog.ui \
         ui/tableguisettings.ui
SOURCES += core/clientsettings.cpp \
           core/filter.cpp \
           core/filtrationwidget.cpp \
           core/generalcoresettings.cpp \
           core/generalguisettings.cpp \
           core/gotolinedialog.cpp \
           core/localizationsettings.cpp \
           core/log.cpp \
           core/logindex.cpp \
           core/logselectdialog.cpp \
           core/main.cpp \
           core/mainsettings.cpp \
           core/mainwindow.cpp \
           core/openconnectiondialog.cpp \
           core/ostools.cpp \
           core/project.cpp \
           core/staticcoreutils.cpp \
           core/staticlogfilter.cpp \
           core/staticlogreader.cpp \
           core/staticlogwriter.cpp \
           shared/emptysettings.cpp \
           shared/staticfromlogselector.cpp \
           shared/staticrecordsreader.cpp \
           visualization/abstract/abstractrealtimetextvisualization.cpp \
           visualization/abstract/abstracttablevisualization.cpp \
           visualization/abstract/abstracttextappearancecolorsandfontssettings.cpp \
           visualization/abstract/abstracttextguisettings.cpp \
           visualization/abstract/abstracttextvisualization.cpp \
           visualization/hexvisualization/hexappearancecolorsandfontssettings.cpp \
           visualization/hexvisualization/hexguisettings.cpp \
           visualization/hexvisualization/hexvisualization.cpp \
           visualization/tablevisualization/tableguisettings.cpp \
           visualization/tablevisualization/tablevisualization.cpp \
           visualization/textvisualization/textappearancecolorsandfontssettings.cpp \
           visualization/textvisualization/textguisettings.cpp \
           visualization/textvisualization/textvisualization.cpp \
           visualization/tools/tableviewer.cpp \
           visualization/tools/viewer.cpp \
           visualization/realtime/hexvisualization/realtimehexvisualization.cpp \
           visualization/realtime/textvisualization/realtimetextvisualization.cpp
RESOURCES += resources/postprocessor.qrc
TRANSLATIONS += resources/localization/postprocessor_en.ts \
                resources/localization/postprocessor_ru.ts \
                resources/localization/qt_en.ts \
                resources/localization/qt_ru.ts
include(../wsnsim.pri)
tr.commands = lrelease $$_PRO_FILE_
QMAKE_EXTRA_TARGETS += tr
POST_TARGETDEPS += tr
