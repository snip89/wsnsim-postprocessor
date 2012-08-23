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
    prName = tr("Simulation logs visualizator");
    delimT1 = tr(" : ");
    delimT2 = tr(" -> ");

    setSettings(settings);

    ui->setupUi(this);

    setWindowTitle(prName);

    createActions();
    createMenus();
    createStatusWidgets();

    insertActionsRecent();

    isProjectOpened = false;
    isLogOpened = false;

    fullScreen = false;

    emptyWidget = new QWidget();

    emptySettings = new EmptySettings();

    generalGuiSettings = new GeneralGuiSettings();
    generalGuiSettings->showCurrentSettings();

    generalCoreSettings = new GeneralCoreSettings();
    generalCoreSettings->showCurrentSettings();

    filtrationWidget = new FiltrationWidget();

    connect(filtrationWidget, SIGNAL(logFiltered(int)), this, SLOT(filteredLog(int)));
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
//    stackedWidget->addWidget(mainSettings);
    stackedWidget->addWidget(hexVisualization->getWidget());
    stackedWidget->addWidget(textVisualization->getWidget());
//    stackedWidget->addWidget(filtrationWidget);

    stackedWidget->setCurrentWidget(emptyWidget);
    activeWidget = EMPTY;
    previousActiveWidget = EMPTY;
}

MainWindow::~MainWindow()
{
    closeProject();

    deleteActions();
    deleteMenus();
    deleteStatusWidgets();

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

void MainWindow::setSettings(QSettings &someSettings)
{
    if(!someSettings.contains("Defaults/General/Gui/Recent_number"))
        someSettings.setValue("Defaults/General/Gui/Recent_number", DEFAULT_RECENT_NUMBER);

    if(!someSettings.contains("General/Gui/Recent_number"))
        someSettings.setValue("General/Gui/Recent_number", DEFAULT_RECENT_NUMBER);

    if(!someSettings.contains("Defaults/General/Gui/File_dialog_path"))
        someSettings.setValue("Defaults/General/Gui/File_dialog_path", QDir::homePath());

    if(!someSettings.contains("General/Gui/File_dialog_path"))
        someSettings.setValue("General/Gui/File_dialog_path", QDir::homePath());

    if(!someSettings.contains("Defaults/General/Gui/Recent"))
    {
        QStringList emptyRecent;
        for(int i = 0; i < DEFAULT_RECENT_NUMBER; i ++)
            emptyRecent.append(" ");

        someSettings.setValue("Defaults/General/Gui/Recent", emptyRecent);
    }

    if(!someSettings.contains("General/Gui/Recent"))
    {
        QStringList emptyRecent;
        for(int i = 0; i < DEFAULT_RECENT_NUMBER; i ++)
            emptyRecent.append(" ");

        someSettings.setValue("General/Gui/Recent", emptyRecent);
    }

    if(!someSettings.contains("Defaults/Hex visualization/Appearance/Colors and Fonts/Cursor_line_color"))
        someSettings.setValue("Defaults/Hex visualization/Appearance/Colors and Fonts/Cursor_line_color", QColor(Qt::yellow).lighter(160));

    if(!someSettings.contains("Hex visualization/Appearance/Colors and Fonts/Cursor_line_color"))
        someSettings.setValue("Hex visualization/Appearance/Colors and Fonts/Cursor_line_color", QColor(Qt::yellow).lighter(160));

    if(!someSettings.contains("Defaults/Text visualization/Appearance/Colors and Fonts/Cursor_line_color"))
        someSettings.setValue("Defaults/Text visualization/Appearance/Colors and Fonts/Cursor_line_color", QColor(Qt::yellow).lighter(160));

    if(!someSettings.contains("Defaults/General/Gui/Default_visualization"))
        someSettings.setValue("Defaults/General/Gui/Default_visualization", "hex");

    if(!someSettings.contains("General/Gui/Default_visualization"))
        someSettings.setValue("General/Gui/Default_visualization", "hex");

    if(!someSettings.contains("Text visualization/Appearance/Colors and Fonts/Cursor_line_color"))
        someSettings.setValue("Text visualization/Appearance/Colors and Fonts/Cursor_line_color", QColor(Qt::yellow).lighter(160));

    if(!someSettings.contains("Hidden/Gui/File_dialog_pos"))
        someSettings.setValue("Hidden/Gui/File_dialog_pos", QPoint(0, 0));

    if(!someSettings.contains("Hidden/Gui/File_dialog_size"))
        someSettings.setValue("Hidden/Gui/File_dialog_size", QSize(320, 240));

    if(!someSettings.contains("Hidden/Gui/Log_dialog_pos"))
        someSettings.setValue("Hidden/Gui/Log_dialog_pos", QPoint(0, 0));

    if(!someSettings.contains("Hidden/Gui/Log_dialog_size"))
        someSettings.setValue("Hidden/Gui/Log_dialog_size", QSize(320, 240));

    if(!someSettings.contains("Hidden/Gui/Line_dialog_pos"))
        someSettings.setValue("Hidden/Gui/Line_dialog_pos", QPoint(0, 0));

    if(!someSettings.contains("Hidden/Gui/Line_dialog_size"))
        someSettings.setValue("Hidden/Gui/Line_dialog_size", QSize(184, 84));

    if(!someSettings.contains("Hidden/Gui/Filter_dialog_pos"))
        someSettings.setValue("Hidden/Gui/Filter_dialog_pos", QPoint(0, 0));

    if(!someSettings.contains("Hidden/Gui/Settings_dialog_pos"))
        someSettings.setValue("Hidden/Gui/Settings_dialog_pos", QPoint(0, 0));
}

void MainWindow::createActions()
{
    actionOpen = new QAction(tr("&Open project..."), this);
    actionOpen->setShortcut(QKeySequence::Open);
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(openProject()));

    actionOpenLog = new QAction(tr("&Open log..."), this);
    actionOpenLog->setEnabled(false);
    connect(actionOpenLog, SIGNAL(triggered()), this, SLOT(openLog()));

    actionClose = new QAction(tr("&Close project"), this);
    actionClose->setShortcut(QKeySequence::Close);
    actionClose->setEnabled(false);
    connect(actionClose, SIGNAL(triggered()), this, SLOT(closeProject()));

    actionPrint = new QAction(tr("&Print..."), this);
    actionPrint->setShortcut(QKeySequence::Print);
    actionPrint->setEnabled(false);

    // actionLogSelect = new QAction(tr("&Select log..."), this);
    // actionLogSelect->setEnabled(false);
    // connect(actionLogSelect, SIGNAL(triggered()), this, SLOT(openLog()));

    actionExit = new QAction(tr("&Exit"), this);
    actionExit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(exit()));

    actionSettings = new QAction(tr("&Settings..."), this);
    connect(actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));

    actionHexVisualization = new QAction(tr("&Hex"), this);
    actionHexVisualization->setEnabled(false);
    actionHexVisualization->setCheckable(true);
    connect(actionHexVisualization, SIGNAL(toggled(bool)), this, SLOT(showHexVisualization(bool)));

    actionTextVisualization = new QAction(tr("&Text"), this);
    actionTextVisualization->setEnabled(false);
    actionTextVisualization->setCheckable(true);
    connect(actionTextVisualization, SIGNAL(toggled(bool)), this, SLOT(showTextVisualization(bool)));

    actionCopy = new QAction(tr("&Copy"), this);
    actionCopy->setShortcut(QKeySequence::Copy);
    actionCopy->setEnabled(false);

    actionPaste = new QAction(tr("&Paste"), this);
    actionPaste->setShortcut(QKeySequence::Paste);
    actionPaste->setEnabled(false);

    actionSelectAll = new QAction(tr("&Select all"), this);
    actionSelectAll->setShortcut(QKeySequence::SelectAll);
    actionSelectAll->setEnabled(false);

    actionFind = new QAction(tr("&Find"), this);
    actionFind->setShortcut(QKeySequence::Find);
    actionFind->setEnabled(false);

    actionFindNext = new QAction(tr("&Find next"), this);
    actionFindNext->setShortcut(QKeySequence::FindNext);
    actionFindNext->setEnabled(false);

    actionFindPrevious = new QAction(tr("&Find previous"), this);
    actionFindPrevious->setShortcut(QKeySequence::FindPrevious);
    actionFindPrevious->setEnabled(false);

    actionGoToLine = new QAction(tr("&Go to line"), this);
    actionGoToLine->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    connect(actionGoToLine, SIGNAL(triggered()), this, SLOT(showGoToLineDialog()));
    actionGoToLine->setEnabled(false);

    actionFiltration = new QAction(tr("&Filter log..."), this);
    actionFiltration->setEnabled(false);
    connect(actionFiltration, SIGNAL(triggered()), this, SLOT(showFiltration()));

    actionFullScreen = new QAction(tr("&Full screen"), this);
    actionFullScreen->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F11));
    actionFullScreen->setCheckable(true);
    connect(actionFullScreen, SIGNAL(toggled(bool)), this, SLOT(toggleFullScreen(bool)));

    actionHelp = new QAction(tr("&Help"), this);

    actionContextHelp = new QAction(tr("&Context help"), this);
    actionContextHelp->setShortcut(QKeySequence::HelpContents);

    actionAbout = new QAction(tr("&About..."), this);
}

void MainWindow::createMenus()
{
//    menuFile = new QMenu(tr("&Файл"), this);
    menuFile = new QMenu(tr("&File"), this);
    menuFile->addAction(actionOpen);
    menuFile->addAction(actionOpenLog);

    menuViewFiltration = new QMenu(tr("&Filtration"), this);

    menuCurrentLog = new QMenu(tr("&Logs"), this);
    menuCurrentLog->setEnabled(false);

    menuViewFiltration->addMenu(menuCurrentLog);

    menuFile->addSeparator();
    menuFile->addAction(actionClose);
    menuFile->addSeparator();

    menuRecentProjects = new QMenu(tr("&Recent projects..."), this);

    menuFile->addMenu(menuRecentProjects);
    menuFile->addSeparator();
    menuFile->addAction(actionPrint);
    menuFile->addSeparator();
    menuFile->addAction(actionExit);

    menuEdit = new QMenu(tr("&Edit"), this);
    menuEdit->addAction(actionCopy);
    menuEdit->addAction(actionPaste);
    menuEdit->addSeparator();
    menuEdit->addAction(actionSelectAll);
    menuEdit->addSeparator();
    menuEdit->addAction(actionFiltration);
    menuEdit->addSeparator();

    menuFind = new QMenu(tr("&Search"), this);
    menuFind->addAction(actionFind);
    menuFind->addSeparator();
    menuFind->addAction(actionFindNext);
    menuFind->addAction(actionFindPrevious);

    menuEdit->addMenu(menuFind);
    menuEdit->addAction(actionGoToLine);

    menuView = new QMenu(tr("&View"), this);
    menuView->addAction(actionHexVisualization);
    menuView->addAction(actionTextVisualization);
    menuView->addSeparator();
    menuView->addMenu(menuViewFiltration);
    menuView->addSeparator();
    menuView->addAction(actionFullScreen);

    menuTools = new QMenu(tr("&Tools"), this);
    menuTools->addAction(actionSettings);

    menuHelp = new QMenu(tr("&Help"), this);
    menuHelp->addAction(actionHelp);
    menuHelp->addAction(actionContextHelp);
    menuHelp->addSeparator();
    menuHelp->addAction(actionAbout);

    ui->menuBar->addMenu(menuFile);
    ui->menuBar->addMenu(menuEdit);
    ui->menuBar->addMenu(menuView);
    ui->menuBar->addMenu(menuTools);
    ui->menuBar->addMenu(menuHelp);
}

void MainWindow::createStatusWidgets()
{
    labelTotalSize = new QLabel(this);
    labelTotalSize->setVisible(false);
    ui->statusbar->addWidget(labelTotalSize);
}

void MainWindow::deleteActions()
{
    delete actionOpen;

    removeActionsRecent();

    // delete actionLogSelect;
    delete actionPrint;
    delete actionClose;
    delete actionExit;
    delete actionSettings;
    delete actionHexVisualization;
    delete actionTextVisualization;
    delete actionFiltration;
    delete actionFullScreen;
    delete actionCopy;
    delete actionPaste;
    delete actionFind;
    delete actionFindNext;
    delete actionFindPrevious;
    delete actionGoToLine;
    delete actionHelp;
    delete actionContextHelp;
    delete actionAbout;
}

void MainWindow::deleteMenus()
{
    delete menuFile;
    delete menuRecentProjects;
    delete menuCurrentLog;
    delete menuEdit;
    delete menuFind;
    delete menuView;
    delete menuTools;
    delete menuHelp;
    delete menuViewFiltration;
}

void MainWindow::deleteStatusWidgets()
{
    delete labelTotalSize;
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

    // menuFile->insertActions(menuCurrentLog, actionsRecent->actions());
    // if(actionsRecent->actions().size() != 0)
    //    menuFile->insertSeparator(actionLogSelect);

    menuRecentProjects->addActions(actionsRecent->actions());
}

void MainWindow::removeActionsRecent()
{
    foreach(QAction *action, actionsRecent->actions())
    {
        menuRecentProjects->removeAction(action);
        delete action;
    }
    delete actionsRecent;
}

void MainWindow::updateActionsCurrentLogMenu()
{
    foreach(QAction *action, menuCurrentLog->actions())
    {
        menuCurrentLog->removeAction(action);
        delete action;
    }

    for(int i = 0; i < logs->size(); i ++)
    {
        QAction *action = new QAction(logs->at(i).fileName, this);
        connect(action, SIGNAL(triggered()), this, SLOT(switchCurrentLog()));
        action->setCheckable(true);
        menuCurrentLog->addAction(action);

        if(i == currentLogId)
        {
            action->setChecked(true);
        }
    }
}

void MainWindow::closeLog()
{
    actionHexVisualization->setEnabled(false);
    actionHexVisualization->setChecked(false);

    actionTextVisualization->setEnabled(false);
    actionTextVisualization->setChecked(false);

    actionFiltration->setEnabled(false);

    switchToWidget(EMPTY);

    if(isLogOpened)
    {
        for(int i = 0; i < logs->size(); i ++)
        {
            Log *log = logs->at(i).log;
            logs->removeAt(i);
            delete log;
        }

        updateActionsCurrentLogMenu();
        filtrationWidget->clearLogs();

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
        labelTotalSize->setVisible(false);
        actionGoToLine->setEnabled(false);
        hexVisualization->activity(false);
        previousActiveWidget = HEXVISUALIZATION;
        break;

    case TEXTVISUALIZATION:
        labelTotalSize->setVisible(false);
        actionGoToLine->setEnabled(false);
        textVisualization->activity(false);
        previousActiveWidget = TEXTVISUALIZATION;
        break;

    case FILTRATION:
        filtrationWidget->deactivate();
        previousActiveWidget = FILTRATION;
        break;
    }

    QString statusString;

    switch(type)
    {
    case EMPTY:
        stackedWidget->setCurrentWidget(emptyWidget);
        activeWidget = EMPTY;
        break;

    case MAINSETTINGS:
//        stackedWidget->setCurrentWidget(mainSettings);
        activeWidget = MAINSETTINGS;
        break;

    case HEXVISUALIZATION:
        statusString += tr("Log size: ");
        statusString += QString::number(logs->at(currentLogId).log->size());
        statusString += tr(" records");
        labelTotalSize->setText(statusString);
        labelTotalSize->setVisible(true);
        actionGoToLine->setEnabled(true);
        stackedWidget->setCurrentWidget(hexVisualization->getWidget());
//        hexVisualization->update(project, logs->at(logs->size() - 1));
        hexVisualization->update(project, logs->at(currentLogId).log);
        hexVisualization->activity(true);
        activeWidget = HEXVISUALIZATION;
        break;

    case TEXTVISUALIZATION:
        statusString += tr("Log size: ");
        statusString += QString::number(logs->at(currentLogId).log->size());
        statusString += tr(" records");
        labelTotalSize->setText(statusString);
        labelTotalSize->setVisible(true);
        actionGoToLine->setEnabled(true);
        stackedWidget->setCurrentWidget(textVisualization->getWidget());
//        textVisualization->update(project, logs->at(logs->size() - 1));
        textVisualization->update(project, logs->at(currentLogId).log);
        textVisualization->activity(true);
        activeWidget = TEXTVISUALIZATION;
        break;

    case FILTRATION:
        filtrationWidget->setCurrentLog(currentLogId);
        filtrationWidget->activate();
//        stackedWidget->setCurrentWidget(filtrationWidget);
        activeWidget = FILTRATION;
        break;
    }
}

void MainWindow::updateVisualization(WidgetType type)
{
    QString statusString;

    switch(type)
    {
    case TEXTVISUALIZATION:
        statusString += tr("Log size: ");
        statusString += QString::number(logs->at(currentLogId).log->size());
        statusString += tr(" records");
        labelTotalSize->setText(statusString);
        labelTotalSize->setVisible(true);
        actionGoToLine->setEnabled(true);
        stackedWidget->setCurrentWidget(textVisualization->getWidget());
        textVisualization->update(project, logs->at(currentLogId).log);
        break;
    case HEXVISUALIZATION:
        statusString += tr("Log size: ");
        statusString += QString::number(logs->at(currentLogId).log->size());
        statusString += tr(" records");
        labelTotalSize->setText(statusString);
        labelTotalSize->setVisible(true);
        actionGoToLine->setEnabled(true);
        stackedWidget->setCurrentWidget(hexVisualization->getWidget());
        hexVisualization->update(project, logs->at(currentLogId).log);
        break;
    case EMPTY:
        break;
    case MAINSETTINGS:
        break;
    case FILTRATION:
        break;
    }
}

bool MainWindow::isFullScreen()
{
    return actionFullScreen->isChecked();
}

void MainWindow::setFullScreen(bool checked)
{
    actionFullScreen->setChecked(checked);
}

void MainWindow::showEmptySettings(QString name)
{
    emptySettings->setSettingsName(name);
    if(!mainSettings->settingsFrameWidget->isAncestorOf(emptySettings))
        mainSettings->settingsFrameWidget->addWidget(emptySettings);

    mainSettings->settingsFrameWidget->setCurrentWidget(emptySettings);
}

void MainWindow::setTitle(QString project, QString log)
{
    if(project != QString::null)
        setWindowTitle(prName + delimT1 + project);

    if(project != QString::null && log != QString::null)
        setWindowTitle(prName + delimT1 + project + delimT2 + log);
}

void MainWindow::openRecentProject()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if(action)
        openProject(action->data().toString());
}

void MainWindow::openProject(QString name)
{
    if(name == QString::null)
    {
        QString dirPath = settings.value("General/Gui/File_dialog_path").toString();

        QFileDialog *fileDialog = new QFileDialog(this, tr("Open project file"), dirPath, tr("XML project files (*xml)"));
        fileDialog->move(settings.value("Hidden/Gui/File_dialog_pos").value<QPoint>());
        fileDialog->resize(settings.value("Hidden/Gui/File_dialog_size").value<QSize>());
        
        if(fileDialog->exec())
            name = fileDialog->selectedFiles().at(0);

        settings.setValue("Hidden/Gui/File_dialog_pos", fileDialog->pos());
        settings.setValue("Hidden/Gui/File_dialog_size", fileDialog->size());

        delete fileDialog;

//        name = fileDialog.getOpenFileName(this, tr("Open project file"), dirPath, tr("XML project files (*xml)"));
//        name = QFileDialog::getOpenFileName(this, tr("Open project file"), dirPath, tr("XML project files (*xml)"));

        if(name == "")
            return;

        QDir dir;
        QFileInfo finfo = QFileInfo(name);

        dirPath = dir.filePath(name);
        dirPath.chop(finfo.fileName().size());
        settings.setValue("General/Gui/File_dialog_path", dirPath);

        if(isProjectOpened)
            closeProject();
    }
    else
    {
        QFile pFile(name);

        if(!pFile.exists())
        {
            errorMessager.showMessage(tr("Project file not found"));
            return;
        }

        if(isProjectOpened)
            closeProject();
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

    setTitle(project->projectName(), QString::null);

    removeActionsRecent();
    insertToRecent(name);
    insertActionsRecent();

    int count = project->logsCount();

    if(count == 0)
    {
        errorMessager.showMessage(tr("Project is empty"));
        delete project;
        return;
    }

    else if(count == 1)
        openLog(project->logName(0));

    else
    {
        int selectedLogId = -1;

        LogSelectDialog *logSelectDialog = new LogSelectDialog(this, project);
        logSelectDialog->move(settings.value("Hidden/Gui/Log_dialog_pos").value<QPoint>());
        logSelectDialog->resize(settings.value("Hidden/Gui/Log_dialog_size").value<QSize>());

        if(logSelectDialog->exec())
            selectedLogId = logSelectDialog->logId();

        settings.setValue("Hidden/Gui/Log_dialog_pos", logSelectDialog->pos());
        settings.setValue("Hidden/Gui/Log_dialog_size", logSelectDialog->size());

        delete logSelectDialog;

        if(selectedLogId != -1)
            openLog(project->logName(selectedLogId));
    }

    actionOpenLog->setEnabled(true);

    // actionLogSelect->setEnabled(true);
    actionClose->setEnabled(true);

    isProjectOpened = true;
}

void MainWindow::closeProject()
{
    if(isProjectOpened)
    {
        if(activeWidget == HEXVISUALIZATION)
            settings.setValue("General/Gui/Default_visualization", "hex");
        else if(activeWidget == TEXTVISUALIZATION)
            settings.setValue("General/Gui/Default_visualization", "text");

        actionClose->setEnabled(false);

        switchToWidget(EMPTY);

        closeLog();

        delete project;

        isProjectOpened = false;

        actionOpenLog->setEnabled(false);
        menuCurrentLog->setEnabled(false);

        setWindowTitle(prName);
    }
}

void MainWindow::openLog(QString name)
{
    if(name == QString::null)
    {
        int selectedLogId = -1;

        LogSelectDialog *logSelectDialog = new LogSelectDialog(this, project);
        logSelectDialog->move(settings.value("Hidden/Gui/Log_dialog_pos").value<QPoint>());
        logSelectDialog->resize(settings.value("Hidden/Gui/Log_dialog_size").value<QSize>());

        if(logSelectDialog->exec())
            selectedLogId = logSelectDialog->logId();

        settings.setValue("Hidden/Gui/Log_dialog_pos", logSelectDialog->pos());
        settings.setValue("Hidden/Gui/Log_dialog_size", logSelectDialog->size());

        delete logSelectDialog;

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

    filtrationWidget->setLogsInfos(logs);

    LogInfo logInfo;

    logInfo.id = currentLogId;
    logInfo.log = new Log(name, blockSize, memoryUsage, info, false);
    logInfo.fileName = logInfo.log->fileName();
    logInfo.filtersInfo.append(tr("No filters"));

    logs->append(logInfo);

    if(!logs->at(currentLogId).log->load(true, false))
    {
        closeLog();
        return;
    }

    filtrationWidget->setMainLog(currentLogId);

    updateActionsCurrentLogMenu();

    // logs->append(new Log(name, blockSize, memoryUsage, info, false));
    // if(!logs->at(logs->size() - 1)->load(true, false))
    // {
    //     errorMessager.showMessage(errorString);
    //     closeLog();
    //     return;
    // }

    logs->at(currentLogId).log->toggleActivity(true);

    setTitle(project->projectName(), logs->at(currentLogId).fileName);

//    logs->at(logs->size() - 1)->toggleActivity(true);

    isLogOpened = true;

    menuCurrentLog->setEnabled(true);

    // TODO: enable all visualization actions here
    actionHexVisualization->setEnabled(true);
    actionTextVisualization->setEnabled(true);

    actionFiltration->setEnabled(true);

//    showHexVisualization(true);
    
    if(settings.value("General/Gui/Default_visualization").value<QString>() == "hex")
        actionHexVisualization->toggle();
    else if(settings.value("General/Gui/Default_visualization").value<QString>() == "text")
        actionTextVisualization->toggle();
}

void MainWindow::showSettings()
{
//    switchToWidget(MAINSETTINGS);
    mainSettings->setWindowFlags(Qt::Dialog);
    mainSettings->setWindowModality(Qt::ApplicationModal);

//    mainSettings->setPos(Hidden/Gui/Settings_dialog_pos
    mainSettings->move(settings.value("Hidden/Gui/Settings_dialog_pos").value<QPoint>());

    mainSettings->show();
}

void MainWindow::showTextVisualization(bool checked)
{
    if(checked)
    {
        if(actionHexVisualization->isChecked())
            actionHexVisualization->setChecked(false);
    
        switchToWidget(TEXTVISUALIZATION);
    }

    if(!checked && activeWidget == TEXTVISUALIZATION)
        switchToWidget(EMPTY);
}

void MainWindow::showHexVisualization(bool checked)
{
    if(checked)
    {
        if(actionTextVisualization->isChecked())
            actionTextVisualization->setChecked(false);
        switchToWidget(HEXVISUALIZATION);
    }

    if(!checked && activeWidget == HEXVISUALIZATION)
        switchToWidget(EMPTY);
}

void MainWindow::showFiltration()
{
    filtrationWidget->setCurrentProject(project);
//    filtrationWidget->setCurrentLog(logs->at(logs->size() - 1));
    filtrationWidget->setCurrentLog(currentLogId);
    filtrationWidget->activate();
//    switchToWidget(FILTRATION);
//    filtrationWidget->show();
    filtrationWidget->setWindowFlags(Qt::Dialog);
    filtrationWidget->setWindowModality(Qt::ApplicationModal);
    filtrationWidget->move(settings.value("Hidden/Gui/Filtration_dialog_pos").value<QPoint>());
    filtrationWidget->show();
}

void MainWindow::showGoToLineDialog()
{
    GoToLineDialog *goToLineDialog = new GoToLineDialog();
    goToLineDialog->move(settings.value("Hidden/Gui/Line_dialog_pos").value<QPoint>());
    goToLineDialog->resize(settings.value("Hidden/Gui/Line_dialog_size").value<QSize>());

    if(goToLineDialog->exec())
    {
        if(activeWidget == HEXVISUALIZATION)
        {
            qint64 number = goToLineDialog->lineNumber();
            if(number < logs->at(currentLogId).log->size() || number >= 0)
                hexVisualization->fromLine(goToLineDialog->lineNumber());
        }
        else if(activeWidget == TEXTVISUALIZATION)
        {   
            qint64 number = goToLineDialog->lineNumber();
            if(number < logs->at(currentLogId).log->size() || number >= 0)
                textVisualization->fromLine(goToLineDialog->lineNumber());
        }
    }

    settings.setValue("Hidden/Gui/Line_dialog_pos", goToLineDialog->pos());
    settings.setValue("Hidden/Gui/Line_dialog_size", goToLineDialog->size());

    delete goToLineDialog;
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

    else if(topLevelName == "Hex visualization")
    {
        if(settingName == "Hex visualization")
            showEmptySettings(tr("Hex visualization"));

        else
        {
            IVisualizationSettings *visualizationSettings = hexVisualization->visualizationSettings(settingName);

            if(!visualizationSettings)
                showEmptySettings(settingName);

            else
            {
                QWidget *widget = visualizationSettings->getWidget();

                visualizationSettings->setSettingsName(settingName);

                if(!mainSettings->settingsFrameWidget->isAncestorOf(widget))
                    mainSettings->settingsFrameWidget->addWidget(widget);
                
                mainSettings->settingsFrameWidget->setCurrentWidget(widget);
            }
        }
    }

    else if(topLevelName == "Text visualization")
    {
        if(settingName == "Text visualization")
            showEmptySettings(tr("Text visualization"));

        else
        {
            IVisualizationSettings *visualizationSettings = textVisualization->visualizationSettings(settingName);


            if(!visualizationSettings)
                showEmptySettings(settingName);

            else
            {
                QWidget *widget = visualizationSettings->getWidget();

                visualizationSettings->setSettingsName(settingName);

                if(!mainSettings->settingsFrameWidget->isAncestorOf(widget))
                    mainSettings->settingsFrameWidget->addWidget(widget);
                
                mainSettings->settingsFrameWidget->setCurrentWidget(widget);
            }
        }
    }
}

void MainWindow::appliedSettings()
{
    generalCoreSettings->applySettings();
    generalGuiSettings->applySettings();

    IVisualizationSettings *hexVisualizationSettings = hexVisualization->visualizationSettings("Colors and Fonts");

    if(hexVisualizationSettings)
        hexVisualizationSettings->applySettings();

    IVisualizationSettings *textVisualizationSettings = textVisualization->visualizationSettings("Colors and Fonts");

    if(textVisualizationSettings)
        textVisualizationSettings->applySettings();

    mainSettings->close();

/*    if(actionHexVisualization->isChecked())
        switchToWidget(HEXVISUALIZATION);

    else if(actionTextVisualization->isChecked())
        switchToWidget(TEXTVISUALIZATION);

    else
    switchToWidget(EMPTY);*/
}

void MainWindow::canceledSettings()
{
    generalCoreSettings->showCurrentSettings();
    generalGuiSettings->showCurrentSettings();

    IVisualizationSettings *hexVisualizationSettings = hexVisualization->visualizationSettings("Colors and Fonts");

    if(hexVisualizationSettings)
        hexVisualizationSettings->showCurrentSettings();

    IVisualizationSettings *textVisualizationSettings = textVisualization->visualizationSettings("Colors and Fonts");

    if(textVisualizationSettings)
        textVisualizationSettings->showCurrentSettings();

/*    if(actionHexVisualization->isChecked())
        switchToWidget(HEXVISUALIZATION);

    else if(actionTextVisualization->isChecked())
        switchToWidget(TEXTVISUALIZATION);

    else
    switchToWidget(EMPTY);*/

    settings.setValue("Hidden/Gui/Settings_dialog_pos", mainSettings->pos());

    mainSettings->close();

    if(activeWidget == TEXTVISUALIZATION)
        updateVisualization(TEXTVISUALIZATION);
    else if(activeWidget == HEXVISUALIZATION)
        updateVisualization(HEXVISUALIZATION);
}

void MainWindow::filteredLog(int id)
{
//    logs->at(logs->size() - 1)->toggleActivity(false);

//    logs->append(newLog);
    currentLogId = id;
    updateActionsCurrentLogMenu();

    filtrationWidget->close();

    if(activeWidget == TEXTVISUALIZATION)
        updateVisualization(TEXTVISUALIZATION);
    else if(activeWidget == HEXVISUALIZATION)
        updateVisualization(HEXVISUALIZATION);

//    qDebug() << logs->at(currentLogId).fileName;

    setTitle(project->projectName(), logs->at(currentLogId).fileName);

//    logs->at(logs->size() - 1)->toggleActivity(true);

/*    if(actionHexVisualization->isChecked())
        switchToWidget(HEXVISUALIZATION);

    else if(actionTextVisualization->isChecked())
        switchToWidget(TEXTVISUALIZATION);

    else
    switchToWidget(EMPTY);*/
}

void MainWindow::canceledFiltration()
{
    settings.setValue("Hidden/Gui/Filtration_dialog_pos", filtrationWidget->pos());

    filtrationWidget->close();
//    if(actionHexVisualization->isChecked())
//        switchToWidget(HEXVISUALIZATION);

//    else if(actionTextVisualization->isChecked())
//        switchToWidget(TEXTVISUALIZATION);

//    else
//        switchToWidget(EMPTY);
}

void MainWindow::toggleFullScreen(bool checked)
{
    if(checked)
    {
        this->showFullScreen();
    }
    else
    {
        this->showNormal();
    }
}

void MainWindow::switchCurrentLog()
{
    QAction *sender = (QAction*)QObject::sender();

    qDebug() << sender;

    for(int i = 0; i < logs->size(); i ++)
    {
        if(logs->at(i).fileName == sender->text())
        {
            logs->at(currentLogId).log->toggleActivity(false);

            currentLogId = i;

            logs->at(currentLogId).log->toggleActivity(true);

            WidgetType tempWidgetType = activeWidget;

            if(tempWidgetType == HEXVISUALIZATION || tempWidgetType == TEXTVISUALIZATION)
                switchToWidget(tempWidgetType);
            
            else if(tempWidgetType == FILTRATION)
            {
                filtrationWidget->setCurrentLog(currentLogId);
            }

            else
                switchToWidget(HEXVISUALIZATION);
        }
    }

    foreach(QAction *action, menuCurrentLog->actions())
    {
        action->setChecked(false);
    }

    sender->setChecked(true);

    setTitle(project->projectName(), logs->at(currentLogId).fileName);
}

void MainWindow::exit()
{
    QApplication::exit(0);
}
