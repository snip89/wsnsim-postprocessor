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
#include <QDebug>

#include "project.h"
#include "log.h"
#include "logselectdialog.h"
#include "simpleeventinfo.h"
#include "textvisualization.h"
#include "hexvisualization.h"
#include "ivisualization.h"
#include "staticlogfilter.h"
#include "filter.h"
#include "booleanoperators.h"
#include "mainsettings.h"
#include "ostools.h"
#include "emptysettings.h"
#include "generalguisettingswidget.h"
#include "generalcoresettingswidget.h"
#include "filtrationwidget.h"

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
    FILTRATION
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private:
    // actions
    QAction *actionOpen;
    QAction *actionClose;
    QActionGroup *actionsRecent;
    QAction *actionLogSelect;
    QAction *actionPrint;
    QAction *actionExit;
    QAction *actionSettings;
    QAction *actionHexVisualization;
    QAction *actionTextVisualization;
    QAction *actionFiltration;

    // menus
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    QMenu *menuTools;
    QMenu *menuHelp;

    QSettings settings;

    QErrorMessage errorMessager;

    bool isProjectOpened;
    bool isLogOpened;

    // visualization and widgets
    WidgetType activeWidget;
    WidgetType previousActiveWidget;
    QWidget *emptyWidget;
    EmptySettings *emptySettings;
    GeneralGuiSettingsWidget *generalGuiSettingsWidget;
    GeneralCoreSettingsWidget *generalCoreSettingsWidget;
    FiltrationWidget *filtrationWidget;
    QStackedWidget *stackedWidget;
    MainSettings *mainSettingsWidget;
    IVisualization *hexVisualization;
    IVisualization *textVisualization;

    Project *project;
    QList<Log*> *logs;

    SimpleEventInfo *info;

    Ui::MainWindow *ui;

    void setSettings(QSettings &settings);

    void createActions();
    void createMenus();

    void deleteActions();
    void deleteMenus();

    void insertActionsRecent();
    void removeActionsRecent();

    void closeLog();

    void insertToRecent(QString fileName);

    void switchToWidget(WidgetType type);

    void showEmptySettings(QString name);

private slots:
    void openRecentProject();
    void openProject(QString name = QString::null);
    void closeProject();

    void openLog(QString name = QString::null);

    void showSettings();
    void showTextVisualization();
    void showHexVisualization();
    void showFiltration();

    void selectedSetting(QString topLevelName, QString settingName);
    void appliedSettings();
    void canceledSettings();

    void filteredLog(Log *newLog);
    void canceledFiltration();

    void exit();
};

#endif /* MAINWINDOW_H_ */
