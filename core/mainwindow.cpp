/*
 * mainwindow.cpp
 *
 *  Created on: May 8, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setSettings(settings);

    ui->setupUi(this);

    createActions();
    createMenus();

    insertActionsRecent();

    isProjectOpened = false;
    isLogOpened = false;

    emptyWidget = new QWidget();

    emptySettings = new EmptySettings();

    generalGuiSettings = new GeneralGuiSettings();
    generalGuiSettings->showCurrentSettings();

    generalCoreSettings = new GeneralCoreSettings();
    generalCoreSettings->showCurrentSettings();

    filtrationWidget = new FiltrationWidget();
    connect(filtrationWidget, SIGNAL(logFiltered(Log*)), this, SLOT(filteredLog(Log*)));
    connect(filtrationWidget, SIGNAL(filtrationCanceled()), this, SLOT(canceledFiltration()));

    hexVisualization = new HexVisualization();
    textVisualization = new TextVisualization();

    mainSettings = new MainSettings();
    connect(mainSettings, SIGNAL(settingSelected(QString, QString)), this, SLOT(selectedSetting(QString, QString)));
    connect(mainSettings, SIGNAL(settingsApplied()), this, SLOT(appliedSettings()));
    connect(mainSettings, SIGNAL(settingsCanceled()), this, SLOT(canceledSettings()));

    stackedWidget = new QStackedWidget(this);
    this->setCentralWidget(stackedWidget);
    stackedWidget->addWidget(emptyWidget);
    stackedWidget->addWidget(mainSettings);
    stackedWidget->addWidget(hexVisualization->getWidget());
    stackedWidget->addWidget(textVisualization->getWidget());
    stackedWidget->addWidget(filtrationWidget);

    stackedWidget->setCurrentWidget(emptyWidget);
    activeWidget = EMPTY;
    previousActiveWidget = EMPTY;
}

MainWindow::~MainWindow()
{
    closeProject();

    deleteActions();
    deleteMenus();

    delete emptyWidget;
    delete emptySettings;
    delete generalGuiSettings;
    delete generalCoreSettings;
    delete filtrationWidget;
    delete hexVisualization;
    delete textVisualization;
    delete mainSettings;
    delete stackedWidget;

    delete ui;
}

void MainWindow::setSettings(QSettings &settings)
{
    if(!settings.contains("Defaults/General/Gui/Recent_number"))
        settings.setValue("Defaults/General/Gui/Recent_number", DEFAULT_RECENT_NUMBER);

    if(!settings.contains("General/Gui/Recent_number"))
        settings.setValue("General/Gui/Recent_number", DEFAULT_RECENT_NUMBER);

    if(!settings.contains("Defaults/General/Gui/File_dialog_path"))
        settings.setValue("Defaults/General/Gui/File_dialog_path", QDir::homePath());

    if(!settings.contains("General/Gui/File_dialog_path"))
        settings.setValue("General/Gui/File_dialog_path", QDir::homePath());

    if(!settings.contains("Defaults/General/Gui/Recent"))
    {
        QStringList emptyRecent;
        for(int i = 0; i < DEFAULT_RECENT_NUMBER; i ++)
            emptyRecent.append(" ");

        settings.setValue("Defaults/General/Gui/Recent", emptyRecent);
    }

    if(!settings.contains("General/Gui/Recent"))
    {
        QStringList emptyRecent;
        for(int i = 0; i < DEFAULT_RECENT_NUMBER; i ++)
            emptyRecent.append(" ");

        settings.setValue("General/Gui/Recent", emptyRecent);
    }
}

void MainWindow::createActions()
{
    actionOpen = new QAction(tr("&Open..."), this);
    actionOpen->setShortcut(QKeySequence::Open);
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(openProject()));

    actionClose = new QAction(tr("&Close"), this);
    actionClose->setShortcut(QKeySequence::Close);
    actionClose->setEnabled(false);
    connect(actionClose, SIGNAL(triggered()), this, SLOT(closeProject()));

    actionPrint = new QAction(tr("&Print..."), this);
    actionPrint->setEnabled(false);

    actionLogSelect = new QAction(tr("&Select log..."), this);
    actionLogSelect->setEnabled(false);
    connect(actionLogSelect, SIGNAL(triggered()), this, SLOT(openLog()));

    actionExit = new QAction(tr("&Exit"), this);
    actionExit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(exit()));

    actionSettings = new QAction(tr("&Settings..."), this);
    connect(actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));

    actionHexVisualization = new QAction(tr("&Hex Visualization"), this);
    actionHexVisualization->setEnabled(false);
    connect(actionHexVisualization, SIGNAL(triggered()), this, SLOT(showHexVisualization()));

    actionTextVisualization = new QAction(tr("&Text Visualization"), this);
    actionTextVisualization->setEnabled(false);
    connect(actionTextVisualization, SIGNAL(triggered()), this, SLOT(showTextVisualization()));

    actionFiltration = new QAction(tr("&Filtration..."), this);
    actionFiltration->setEnabled(false);
    connect(actionFiltration, SIGNAL(triggered()), this, SLOT(showFiltration()));
}

void MainWindow::createMenus()
{
    menuFile = new QMenu(tr("&File"), this);
    menuFile->addAction(actionOpen);
    menuFile->addSeparator();
    menuFile->addAction(actionClose);
    menuFile->addSeparator();
    menuFile->addAction(actionPrint);
    menuFile->addSeparator();
    menuFile->addAction(actionLogSelect);
    menuFile->addSeparator();
    menuFile->addAction(actionExit);

    menuEdit = new QMenu(tr("&Edit"), this);
    menuEdit->addAction(actionFiltration);

    menuView = new QMenu(tr("&View"), this);
    menuView->addAction(actionHexVisualization);
    menuView->addAction(actionTextVisualization);

    // TODO: rename to "Options" and menuOptions
    menuTools = new QMenu(tr("&Tools"), this);
    menuTools->addAction(actionSettings);

    menuHelp = new QMenu(tr("&Help"), this);

    ui->menuBar->addMenu(menuFile);
    ui->menuBar->addMenu(menuEdit);
    ui->menuBar->addMenu(menuView);
    ui->menuBar->addMenu(menuTools);
    ui->menuBar->addMenu(menuHelp);
}

void MainWindow::deleteActions()
{
    delete actionOpen;

    removeActionsRecent();

    delete actionLogSelect;
    delete actionPrint;
    delete actionClose;
    delete actionExit;
    delete actionSettings;
    delete actionHexVisualization;
    delete actionTextVisualization;
    delete actionFiltration;
}

void MainWindow::deleteMenus()
{
    delete menuFile;
    delete menuEdit;
    delete menuView;
    delete menuTools;
    delete menuHelp;
}

void MainWindow::insertActionsRecent()
{
    actionsRecent = new QActionGroup(this);
    QStringList recentFilesNames = settings.value("General/Gui/Recent").toStringList();
    for(int i = 0; i < recentFilesNames.size(); i ++)
    {
        if(recentFilesNames[i] != " ")
        {
            QString recentFileName = QFileInfo(recentFilesNames[i]).fileName();
            QAction *action = new QAction(tr(("&" + QString::number(i + 1) + " " + recentFileName).toAscii().data()), this);
            action->setData(recentFilesNames[i]);
            actionsRecent->addAction(action);
            connect(actionsRecent->actions()[i], SIGNAL(triggered()), this, SLOT(openRecentProject()));
        }
    }

    menuFile->insertActions(actionLogSelect, actionsRecent->actions());
    if(actionsRecent->actions().size() != 0)
        menuFile->insertSeparator(actionLogSelect);
}

void MainWindow::removeActionsRecent()
{
    if(!actionsRecent->actions().size() == 0)
        menuFile->removeAction(menuFile->actions()[menuFile->actions().size() - 5]);

    foreach(QAction *action, actionsRecent->actions())
    {
        menuFile->removeAction(action);
        delete action;
    }
    delete actionsRecent;
}

void MainWindow::closeLog()
{
    actionHexVisualization->setEnabled(false);
    actionTextVisualization->setEnabled(false);

    actionFiltration->setEnabled(false);

    switchToWidget(EMPTY);

    if(isLogOpened)
    {
        for(int i = 0; i < logs->size(); i ++)
        {
            delete logs->at(i);
        }
        delete logs;

        isLogOpened = false;
    }
}

void MainWindow::insertToRecent(QString fileName)
{
    int recentNumber = settings.value("General/Gui/Recent_number").toInt();

    bool repeated = false;
    bool inserted = false;
    QStringList recentNames = settings.value("General/Gui/Recent").toStringList();
    for(int i = 0; i < recentNumber; i ++)
    {
        if(recentNames[i] == " ")
        {
            recentNames[i] = fileName;
            inserted = true;
            break;
        }
        else if(recentNames[i] == fileName)
        {
            repeated = true;
            break;
        }
    }

    if(!inserted && !repeated)
    {
        for(int i = 0; i < recentNumber; i ++)
        {
            if(i == recentNumber - 1)
            {
                recentNames[i] = fileName;
            }
            else
                recentNames[i] = recentNames[i + 1];
        }
    }

    settings.setValue("General/Gui/Recent", recentNames);
}

void MainWindow::switchToWidget(WidgetType type)
{
    switch(activeWidget)
    {
    case EMPTY:
        previousActiveWidget = EMPTY;
        break;

    case MAINSETTINGS:
        previousActiveWidget = MAINSETTINGS;
        break;

    case HEXVISUALIZATION:
        hexVisualization->activity(false);
        previousActiveWidget = HEXVISUALIZATION;
        break;

    case TEXTVISUALIZATION:
        textVisualization->activity(false);
        previousActiveWidget = TEXTVISUALIZATION;
        break;

    case FILTRATION:
        filtrationWidget->deactivate();
        previousActiveWidget = FILTRATION;
        break;
    }

    switch(type)
    {
    case EMPTY:
        stackedWidget->setCurrentWidget(emptyWidget);
        activeWidget = EMPTY;
        break;

    case MAINSETTINGS:
        stackedWidget->setCurrentWidget(mainSettings);
        activeWidget = MAINSETTINGS;
        break;

    case HEXVISUALIZATION:
        stackedWidget->setCurrentWidget(hexVisualization->getWidget());
//        hexVisualization->update(project, logs->at(logs->size() - 1));
        hexVisualization->update(project, logs->at(currentLogId));
        hexVisualization->activity(true);
        activeWidget = HEXVISUALIZATION;
        break;

    case TEXTVISUALIZATION:
        stackedWidget->setCurrentWidget(textVisualization->getWidget());
//        textVisualization->update(project, logs->at(logs->size() - 1));
        textVisualization->update(project, logs->at(currentLogId));
        textVisualization->activity(true);
        activeWidget = TEXTVISUALIZATION;
        break;

    case FILTRATION:
        stackedWidget->setCurrentWidget(filtrationWidget);
        activeWidget = FILTRATION;
        break;
    }
}

void MainWindow::showEmptySettings(QString name)
{
    emptySettings->setSettingsName(name);
    if(!mainSettings->settingsFrameWidget->isAncestorOf(emptySettings))
        mainSettings->settingsFrameWidget->addWidget(emptySettings);

    mainSettings->settingsFrameWidget->setCurrentWidget(emptySettings);
}

void MainWindow::openRecentProject()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if(action)
        openProject(action->data().toString());
}

void MainWindow::openProject(QString name)
{
    if(isProjectOpened)
        closeProject();

    if(name == QString::null)
    {
        QString dirPath = settings.value("General/Gui/File_dialog_path").toString();

        name = QFileDialog::getOpenFileName(this, tr("Open project file"), dirPath, tr("XML project files (*xml)"));

        if(name == "")
            return;

        QDir dir;
        QFileInfo finfo = QFileInfo(name);

        dirPath = dir.filePath(name);
        dirPath.chop(finfo.fileName().size());
        settings.setValue("General/Gui/File_dialog_path", dirPath);
    }
    else
    {
        QFile pFile(name);

        if(!pFile.exists())
        {
            errorMessager.showMessage("Project not found");
            return;
        }
    }

    QString errorString;

    project = new Project(name);
    project->load(errorString);

    if(!errorString.isNull())
    {
        errorMessager.showMessage(errorString);
        delete project;
        return;
    }

    removeActionsRecent();
    insertToRecent(name);
    insertActionsRecent();

    int count = project->logsCount();

    if(count == 0)
    {
        errorMessager.showMessage("Project is empty");
        delete project;
        return;
    }

    else if(count == 1)
        openLog(project->logName(0));

    else
    {
        int selectedLogId = LogSelectDialog::selectedLogId(this, project);
        if(selectedLogId != -1)
            openLog(project->logName(selectedLogId));
    }

    actionLogSelect->setEnabled(true);
    actionClose->setEnabled(true);

    isProjectOpened = true;
}

void MainWindow::closeProject()
{
    if(isProjectOpened)
    {
        actionClose->setEnabled(false);

        switchToWidget(EMPTY);

        closeLog();

        delete project;

        isProjectOpened = false;

        actionLogSelect->setEnabled(false);
    }
}

void MainWindow::openLog(QString name)
{
    if(name == QString::null)
    {
        int selectedLogId = LogSelectDialog::selectedLogId(this, project);
        if(selectedLogId != -1)
        {
            if(isLogOpened)
                closeLog();
            openLog(project->logName(selectedLogId));
        }

        return;
    }

    QFileInfo projectFileInfo(project->projectName());
    QDir::setCurrent(projectFileInfo.absoluteDir().absolutePath());

    QString errorString;

    int eventsInfoSize = 0;
    info = project->info(eventsInfoSize);

    qint64 blockSize = settings.value("General/Core/Block_size").toLongLong();

    int memoryUsagePercent = settings.value("General/Core/Memory_usage").toInt();
    qint64 memoryUsage = OSTools::getTotalSystemMemory() * memoryUsagePercent / 100;

//    logs = new QList<Log*>();

    currentLogId = 0;

    logs = new QList<LogInfo>();

    LogInfo logInfo;

    logInfo.id = currentLogId;
    logInfo.log = new Log(name, blockSize, memoryUsage, info, false);
    logInfo.fileName = logInfo.log->fileName();

    logs->append(logInfo);

    if(!logs->at(currentLogId)->load(true, false))
    {
        closeLog();
        return;
    }

    // logs->append(new Log(name, blockSize, memoryUsage, info, false));
    // if(!logs->at(logs->size() - 1)->load(true, false))
    // {
    //     errorMessager.showMessage(errorString);
    //     closeLog();
    //     return;
    // }

    logs->at(currentLogId)->toggleActivity(true);

//    logs->at(logs->size() - 1)->toggleActivity(true);

    isLogOpened = true;

    // TODO: enable all visualization actions here
    actionHexVisualization->setEnabled(true);
    actionTextVisualization->setEnabled(true);

    actionFiltration->setEnabled(true);

    showHexVisualization();
}

void MainWindow::showSettings()
{
    switchToWidget(MAINSETTINGS);
}

void MainWindow::showTextVisualization()
{
    switchToWidget(TEXTVISUALIZATION);
}

void MainWindow::showHexVisualization()
{
    switchToWidget(HEXVISUALIZATION);
}

void MainWindow::showFiltration()
{
    filtrationWidget->setCurrentProject(project);
//    filtrationWidget->setCurrentLog(logs->at(logs->size() - 1));
    filtrationWidget->setCurrentLog(logs->at(currentLogId));
    filtrationWidget->activate();
    switchToWidget(FILTRATION);
}

void MainWindow::selectedSetting(QString topLevelName, QString settingName)
{
    if(topLevelName == "General")
    {
        if(settingName == "General")
        {
            showEmptySettings(settingName);
        }

        else if(settingName == "Gui")
        {
            generalGuiSettings->setSettingsName(settingName);

            if(!mainSettings->settingsFrameWidget->isAncestorOf(generalGuiSettings))
                mainSettings->settingsFrameWidget->addWidget(generalGuiSettings);

            mainSettings->settingsFrameWidget->setCurrentWidget(generalGuiSettings);
        }

        else if(settingName == "Core")
        {
            generalCoreSettings->setSettingsName(settingName);

            if(!mainSettings->settingsFrameWidget->isAncestorOf(generalCoreSettings))
                mainSettings->settingsFrameWidget->addWidget(generalCoreSettings);

            mainSettings->settingsFrameWidget->setCurrentWidget(generalCoreSettings);
        }

        else
        {
            showEmptySettings(settingName);
        }
    }
}

void MainWindow::appliedSettings()
{
    generalCoreSettings->applySettings();
    generalGuiSettings->applySettings();

    switchToWidget(previousActiveWidget);
}

void MainWindow::canceledSettings()
{
    generalCoreSettings->showCurrentSettings();
    generalGuiSettings->showCurrentSettings();

    switchToWidget(previousActiveWidget);
}

void MainWindow::filteredLog(Log *newLog)
{
//    logs->at(logs->size() - 1)->toggleActivity(false);

    logs->append(newLog);

//    logs->at(logs->size() - 1)->toggleActivity(true);

    switchToWidget(previousActiveWidget);
}

void MainWindow::canceledFiltration()
{
    switchToWidget(previousActiveWidget);
}

void MainWindow::exit()
{
    QApplication::exit(0);
}
