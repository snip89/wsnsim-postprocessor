/*
 * mainwindow.h
 *
 *  Created on: May 8, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QApplication>
#include <QString>
#include <QChar>
#include <QStringList>
#include <QStackedWidget>
#include <QSettings>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QKeySequence>
#include <QErrorMessage>
#include <QIcon>
#include <QList>
#include <QStyle>
#include <QPoint>
#include <QSize>
#include <QLabel>
#include <QUdpSocket>
#include <QDebug>
#include <QPair>

#include "project.h"
#include "log.h"
#include "simpleeventinfo.h"
#include "textvisualization.h"
#include "hexvisualization.h"
#include "tablevisualization.h"
#include "ivisualization.h"
#include "staticlogfilter.h"
#include "filter.h"
#include "booleanoperators.h"
#include "mainsettings.h"
#include "ostools.h"
#include "filtrationwidget.h"
#include "loginfo.h"
#include "gotolinedialog.h"
#include "realtimetextvisualization.h"
#include "realtimehexvisualization.h"
#include "realtimetablevisualization.h"
#include "irealtimevisualization.h"
#include "openconnectiondialog.h"
#include "staticcoreutils.h"
#include "udpsocketadapter.h"
#include "formatacceptingdialog.h"
#include "staticvisualizationtools.h"
#include "columnsselectiondialog.h"
#include "formatvalidator.h"

#define DEFAULT_RECENT_NUMBER 4

namespace Ui {
    class MainWindow;
}

enum WidgetType
{
    EMPTY,
    MAINSETTINGS,
    HEXVISUALIZATION,
    TEXTVISUALIZATION,
    TABLEVISUALIZATION,
    RTTEXTVISUALIZATION,
    RTHEXVISUALIZATION,
    RTTABLEVISUALIZATION,
    FILTRATION
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    bool isFullScreen();
    void setFullScreen(bool checked);

    virtual ~MainWindow();

private:
    bool realTime;

    // actions
    QAction *actionOpen;
    QAction *actionOpenConnection;
    QAction *actionClose;
    QActionGroup *actionsRecent;
    QAction *actionPrint;
    QAction *actionExit;
    QAction *actionSettings;
    QAction *actionHexVisualization;
    QAction *actionTextVisualization;
    QAction *actionTableVisualization;
    QAction *actionCopy;
    //QAction *actionPaste;
    QAction *actionSelectAll;
    QAction *actionFind;
    QAction *actionFindNext;
    QAction *actionFindPrevious;
    QAction *actionGoToLine;

    QAction *actionAcceptFormat;
    QAction *actionClearFormat;

    QAction *actionSelectColumns;

    // FIXME: rename this later
    QAction *actionFiltration;

    QAction *actionFullScreen;
    QAction *actionHelp;
    //QAction *actionContextHelp;
    QAction *actionAbout;

    // menus
    QMenu *menuFile;
    QMenu *menuRecentProjects;
    //QMenu *menuCurrentLog;
    QMenu *menuEdit;
    QMenu *menuFind;
    QMenu *menuView;
    QMenu *menuTools;
    QMenu *menuHelp;
    //QMenu *menuViewFiltration;

    QMenu *menuFiltrationLogs;

    QLabel *labelTotalSize;

    QSettings settings;

    QErrorMessage errorMessager;

    QString prName;
    QString delimT1;
    QString delimT2;

    int currentLogId;

    bool isProjectOpened;
    bool isLogOpened;

    // visualization and widgets
    WidgetType activeWidget;
    WidgetType previousActiveWidget;
    QWidget *emptyWidget;
    FiltrationWidget *filtrationWidget;
    QStackedWidget *stackedWidget;
    MainSettings *mainSettings;
    IVisualization *hexVisualization;
    IVisualization *textVisualization;
    IVisualization *tableVisualization;

    IRealTimeVisualization *realTimeTextVisualization;
    IRealTimeVisualization *realTimeHexVisualization;
    IRealTimeVisualization *realTimeTableVisualization;

    bool hexUpdated;
    bool textUpdated;
    bool tableUpdated;

    bool rtHexUpdated;
    bool rtTextUpdated;
    bool rtTableUpdated;

    Project *project;
    UdpSocketAdapter *socketAdapter;

    // Format *format;
    QList<Format*> formats;

    // TODO: remove after implemention new logs list
//    QList<Log*> *logs;

    QList<LogInfo> *logs;

    SimpleEventInfo *info;

    bool fullScreen;

    Ui::MainWindow *ui;

    void setSettings(QSettings &someSettings);

    void createActions();
    void createMenus();
    void initToolBar();
    void createStatusWidgets();

    void deleteActions();
    void deleteMenus();
    void deleteStatusWidgets();

    void insertActionsRecent();
    void removeActionsRecent();

    void updateActionsCurrentLogMenu();

    void closeLog();

    void insertToRecent(QString fileName);

    void switchToWidget(WidgetType type);

    // void showEmptySettings(QString name);

    void setTitle(QString project, QString log);

public slots:
    void openProject(QString name = QString::null);
    void openConnection();

private slots:
    void updateVisualization(WidgetType type);

    void openRecentProject();

    void closeProject();
    void closeConnection();

    void closeAnything();

    void openLog(QString name);

    void showSettings();
    void showTextVisualization(bool checked);
    void showHexVisualization(bool checked);
    void showTableVisualization(bool checked);
    void showFiltration();
    void showGoToLineDialog();

    void loadFormat();
    void clearFormat();

    // void selectedSetting(QString topLevelName, QString settingName);
    void appliedSettings();
    // void canceledSettings();

    void filteredLog(int id);
    // void canceledFiltration();

    void toggleFullScreen(bool checked);

    void switchCurrentLog();

    void showColumnsSelectionDialog();

    void exit();
};

#endif /* MAINWINDOW_H_ */
