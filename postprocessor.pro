######################################################################
# Automatically generated by qmake (2.01a) Thu Jun 7 21:44:55 2012
######################################################################

QT += core gui xml
INCLUDEPATH += ../libs/projectData
TEMPLATE = app
TARGET = 
DEPENDPATH += . \
              core \
              interfaces \
              shared \
              ui \
              core/include \
              shared/include \
              visualization/abstract \
              visualization/hexvisualization \
              visualization/textvisualization \
              visualization/tools \
              visualization/abstract/include \
              visualization/hexvisualization/include \
              visualization/textvisualization/include \
              visualization/tools/include
INCLUDEPATH += . \
               core/include \
               interfaces \
               shared/include \
               visualization/textvisualization/include \
               visualization/abstract/include \
               visualization/tools/include \
               visualization/hexvisualization/include

# Input
HEADERS += interfaces/ilog.h \
           interfaces/iproject.h \
           interfaces/ivisualization.h \
           interfaces/ivisualizationsettings.h \
           ui/ui_hexvisualizationtextandcolorsettings.h \
           core/include/abstractfilter.h \
           core/include/booleanoperators.h \
           core/include/filter.h \
           core/include/filtrationwidget.h \
           core/include/generalcoresettings.h \
           core/include/generalguisettings.h \
           core/include/gotolinedialog.h \
           core/include/log.h \
           core/include/logindex.h \
           core/include/loginfo.h \
           core/include/logselectdialog.h \
           core/include/mainsettings.h \
           core/include/mainwindow.h \
           core/include/ostools.h \
           core/include/project.h \
           core/include/staticlogfilter.h \
           core/include/staticlogreader.h \
           core/include/staticlogwriter.h \
           shared/include/emptysettings.h \
           shared/include/logdatatypes.h \
           shared/include/records.h \
           shared/include/simpleeventinfo.h \
           shared/include/staticrecordsreader.h \
           visualization/abstract/include/abstracttextappearancecolorsandfontssettings.h \
           visualization/abstract/include/abstracttextvisualization.h \
           visualization/hexvisualization/include/hexappearancecolorsandfontssettings.h \
           visualization/hexvisualization/include/hexvisualization.h \
           visualization/textvisualization/include/textappearancecolorsandfontssettings.h \
           visualization/textvisualization/include/textvisualization.h \
           visualization/tools/include/viewer.h
FORMS += ui/abstracttextappearancecolorsandfontssettings.ui \
         ui/abstracttextvisualization.ui \
         ui/emptysettings.ui \
         ui/filtrationwidget.ui \
         ui/generalcoresettings.ui \
         ui/generalguisettings.ui \
         ui/gotolinedialog.ui \
         ui/hexappearancecolorsandfontssettings.ui \
         ui/logselectdialog.ui \
         ui/mainsettings.ui \
         ui/mainwindow.ui
SOURCES += core/filter.cpp \
           core/filtrationwidget.cpp \
           core/generalcoresettings.cpp \
           core/generalguisettings.cpp \
           core/gotolinedialog.cpp \
           core/log.cpp \
           core/logindex.cpp \
           core/logselectdialog.cpp \
           core/main.cpp \
           core/mainsettings.cpp \
           core/mainwindow.cpp \
           core/ostools.cpp \
           core/project.cpp \
           core/staticlogfilter.cpp \
           core/staticlogreader.cpp \
           core/staticlogwriter.cpp \
           shared/emptysettings.cpp \
           shared/staticrecordsreader.cpp \
           visualization/abstract/abstracttextappearancecolorsandfontssettings.cpp \
           visualization/abstract/abstracttextvisualization.cpp \
           visualization/hexvisualization/hexappearancecolorsandfontssettings.cpp \
           visualization/hexvisualization/hexvisualization.cpp \
           visualization/textvisualization/textappearancecolorsandfontssettings.cpp \
           visualization/textvisualization/textvisualization.cpp \
           visualization/tools/viewer.cpp
include(../wsnsim.pri)
