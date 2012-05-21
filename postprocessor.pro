######################################################################
# Automatically generated by qmake (2.01a) Mon May 21 16:49:53 2012
######################################################################

QT += core gui xml
TARGET = postprocessor
DESTDIR = ../bin
OBJECTS_DIR = obj
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
              visualization/abstractvisualizations \
              visualization/hexvisualization \
              visualization/textvisualization \
              visualization/abstractvisualizations/include \
              visualization/hexvisualization/include \
              visualization/textvisualization/include
INCLUDEPATH += . \
               core/include \
               interfaces \
               shared/include \
               visualization/textvisualization/include \
               visualization/abstractvisualizations/include \
               visualization/hexvisualization/include

# Input
HEADERS += interfaces/ilog.h \
           interfaces/iproject.h \
           interfaces/ivisualization.h \
           core/include/abstractfilter.h \
           core/include/booleanoperators.h \
           core/include/filter.h \
           core/include/filtrationwidget.h \
           core/include/generalcoresettingswidget.h \
           core/include/generalguisettingswidget.h \
           core/include/log.h \
           core/include/logindex.h \
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
           visualization/abstractvisualizations/include/abstracttextvisualization.h \
           visualization/hexvisualization/include/hexvisualization.h \
           visualization/textvisualization/include/textvisualization.h
FORMS += ui/abstracttextvisualization.ui \
         ui/emptysettings.ui \
         ui/filtrationwidget.ui \
         ui/generalcoresettingswidget.ui \
         ui/generalguisettingswidget.ui \
         ui/hexvisualization.ui \
         ui/logselectdialog.ui \
         ui/mainsettings.ui \
         ui/mainwindow.ui \
         ui/textvisualization.ui
SOURCES += core/filter.cpp \
           core/filtrationwidget.cpp \
           core/generalcoresettingswidget.cpp \
           core/generalguisettingswidget.cpp \
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
           visualization/abstractvisualizations/abstracttextvisualization.cpp \
           visualization/hexvisualization/hexvisualization.cpp \
           visualization/textvisualization/textvisualization.cpp
