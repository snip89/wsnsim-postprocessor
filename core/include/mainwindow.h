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
#include <QList>
#include <QStyle>
#include <QPoint>
#include <QSize>
#include <QLabel>
#include <QUdpSocket>
#include <QDebug>

#include "project.h"
#include "log.h"
#include "logselectdialog.h"
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
// #include "emptysettings.h"
// #include "generalguisettings.h"
// #include "generalcoresettings.h"
#include "filtrationwidget.h"
#include "loginfo.h"
#include "gotolinedialog.h"
// #include "localizationsettings.h"
#include "realtimetextvisualization.h"

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
    QAction *actionOpenLog;
    QAction *actionOpenConnection;
    QAction *actionClose;
    QAction *actionCloseConnection;
    QActionGroup *actionsRecent;
    QAction *actionPrint;
    QAction *actionExit;
    QAction *actionSettings;
    QAction *actionHexVisualization;
    QAction *actionTextVisualization;
    QAction *actionTableVisualization;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionSelectAll;
    QAction *actionFind;
    QAction *actionFindNext;
    QAction *actionFindPrevious;
    QAction *actionGoToLine;

    // FIXME: rename this later
    QAction *actionFiltration;

    QAction *actionFullScreen;
    QAction *actionHelp;
    QAction *actionContextHelp;
    QAction *actionAbout;

    // menus
    QMenu *menuFile;
    QMenu *menuRecentProjects;
    QMenu *menuCurrentLog;
    QMenu *menuEdit;
    QMenu *menuFind;
    QMenu *menuView;
    QMenu *menuTools;
    QMenu *menuHelp;
    QMenu *menuViewFiltration;

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
    // EmptySettings *emptySettings;
    // LocalizationSettings *localizationSettings;
    // GeneralGuiSettings *generalGuiSettings;
    // GeneralCoreSettings *generalCoreSettings;
    FiltrationWidget *filtrationWidget;
    QStackedWidget *stackedWidget;
    MainSettings *mainSettings;
    IVisualization *hexVisualization;
    IVisualization *textVisualization;
    IVisualization *tableVisualization;

    bool hexUpdated;
    bool textUpdated;
    bool tableUpdated;

    Project *project;

    // TODO: remove after implemention new logs list
//    QList<Log*> *logs;

    QList<LogInfo> *logs;

    SimpleEventInfo *info;

    bool fullScreen;

    Ui::MainWindow *ui;

    void setSettings(QSettings &someSettings);

    void createActions();
    void createMenus();
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

private slots:
    void updateVisualization(WidgetType type);

    void openRecentProject();

    void closeProject();

    void openLog(QString name = QString::null);

    void showSettings();
    void showTextVisualization(bool checked);
    void showHexVisualization(bool checked);
    void showTableVisualization(bool checked);
    void showFiltration();
    void showGoToLineDialog();

    // void selectedSetting(QString topLevelName, QString settingName);
    void appliedSettings();
    // void canceledSettings();

    void filteredLog(int id);
    // void canceledFiltration();

    void toggleFullScreen(bool checked);

    void switchCurrentLog();

    void exit();
};

#endif /* MAINWINDOW_H_ */
