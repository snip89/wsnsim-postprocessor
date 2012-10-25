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
    prName = tr("Logs visualizer");
    delimT1 = tr(" : ");
    delimT2 = tr(" -> ");

    hexUpdated = false;
    textUpdated = false;
    tableUpdated = false;

    setSettings(settings);

    ui->setupUi(this);

    setWindowTitle(prName);

    realTime = false;

    createActions();
    createMenus();
    createStatusWidgets();

    insertActionsRecent();

    isProjectOpened = false;
    isLogOpened = false;

    fullScreen = false;

    emptyWidget = new QWidget();

    filtrationWidget = new FiltrationWidget();

    connect(filtrationWidget, SIGNAL(logFiltered(int)), this, SLOT(filteredLog(int)));

    hexVisualization = new HexVisualization();
    textVisualization = new TextVisualization();
    tableVisualization = new TableVisualization();

    realTimeTextVisualization = new RealTimeTextVisualization();
    realTimeHexVisualization = new RealTimeHexVisualization();
    realTimeTableVisualization = new RealTimeTableVisualization();

    mainSettings = new MainSettings();

    connect(mainSettings, SIGNAL(settingsApplied()), this, SLOT(appliedSettings()));

    stackedWidget = new QStackedWidget(this);
    this->setCentralWidget(stackedWidget);
    stackedWidget->addWidget(emptyWidget);

    stackedWidget->addWidget(hexVisualization->getWidget());
    stackedWidget->addWidget(textVisualization->getWidget());
    stackedWidget->addWidget(tableVisualization->getWidget());

    stackedWidget->addWidget(realTimeTextVisualization->getWidget());
    stackedWidget->addWidget(realTimeHexVisualization->getWidget());
    stackedWidget->addWidget(realTimeTableVisualization->getWidget());

    stackedWidget->setCurrentWidget(emptyWidget);

    activeWidget = EMPTY;
    previousActiveWidget = EMPTY;
}

MainWindow::~MainWindow()
{
    closeProject();
    closeConnection();

    deleteActions();
    deleteMenus();
    deleteStatusWidgets();

    delete emptyWidget;
    delete filtrationWidget;
    delete hexVisualization;
    delete textVisualization;
    delete tableVisualization;

    delete realTimeTextVisualization;
    delete realTimeHexVisualization;
    delete realTimeTableVisualization;

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

    if(!someSettings.contains("Defaults/General/Gui/Default_visualization"))
        someSettings.setValue("Defaults/General/Gui/Default_visualization", "hex");

    if(!someSettings.contains("General/Gui/Default_visualization"))
        someSettings.setValue("General/Gui/Default_visualization", "hex");

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

    if(!someSettings.contains("Hidden/Gui/Font_dialog_pos"))
        someSettings.setValue("Hidden/Gui/Font_dialog_pos", QPoint(0, 0));

    if(!someSettings.contains("Hidden/Gui/Font_dialog_size"))
        someSettings.setValue("Hidden/Gui/Font_dialog_size", QSize(320, 240));

    if(!someSettings.contains("Hidden/Gui/Color_dialog_size"))
        someSettings.setValue("Hidden/Gui/Color_dialog_size", QPoint(320, 240));
}

void MainWindow::createActions()
{
    actionOpen = new QAction(tr("&Open project..."), this);
    actionOpen->setShortcut(QKeySequence::Open);
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(openProject()));

    actionOpenConnection = new QAction(tr("&Open connection..."), this);
    actionOpenConnection->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    connect(actionOpenConnection, SIGNAL(triggered()), this, SLOT(openConnection()));

    actionClose = new QAction(tr("&Close"), this);
    actionClose->setShortcut(QKeySequence::Close);
    actionClose->setEnabled(false);
    connect(actionClose, SIGNAL(triggered()), this, SLOT(closeAnything()));

    actionPrint = new QAction(tr("&Print..."), this);
    actionPrint->setShortcut(QKeySequence::Print);
    actionPrint->setEnabled(false);

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

    actionTableVisualization = new QAction(tr("&Table"), this);
    actionTableVisualization->setEnabled(false);
    actionTableVisualization->setCheckable(true);
    connect(actionTableVisualization, SIGNAL(toggled(bool)), this, SLOT(showTableVisualization(bool)));

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
    menuFile = new QMenu(tr("&File"), this);
    menuFile->addAction(actionOpen);
    menuFile->addAction(actionOpenConnection);

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
    menuFind->setEnabled(false);

    menuEdit->addMenu(menuFind);
    menuEdit->addAction(actionGoToLine);

    menuView = new QMenu(tr("&View"), this);
    menuView->addAction(actionHexVisualization);
    menuView->addAction(actionTextVisualization);
    menuView->addAction(actionTableVisualization);
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
    delete actionOpenConnection;

    removeActionsRecent();

    delete actionPrint;
    delete actionClose;
    delete actionExit;
    delete actionSettings;
    delete actionHexVisualization;
    delete actionTextVisualization;
    delete actionTableVisualization;
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
            QAction *action = new QAction(("&" + QString::number(i + 1) + " " + recentFileName.toAscii().data()), this);
            action->setData(recentFilesNames[i]);
            actionsRecent->addAction(action);
            connect(actionsRecent->actions()[i], SIGNAL(triggered()), this, SLOT(openRecentProject()));
        }
    }

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

    actionTableVisualization->setEnabled(false);
    actionTableVisualization->setChecked(false);

    actionFiltration->setEnabled(false);

    switchToWidget(EMPTY);

    hexUpdated = false;
    textUpdated = false;
    tableUpdated = false;

    if(isLogOpened)
    {
        for(int i = 0; i < logs->size(); i ++)
        {
            Log *log = logs->at(i).log;
            // logs->removeAt(i);
            delete log;
        }

        logs->clear();

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

    case TABLEVISUALIZATION:
        labelTotalSize->setVisible(false);
        actionGoToLine->setEnabled(false);
        tableVisualization->activity(false);
        previousActiveWidget = TABLEVISUALIZATION;
        break;

    case RTTEXTVISUALIZATION:
        previousActiveWidget = RTTEXTVISUALIZATION;
        break;

    case RTHEXVISUALIZATION:
        previousActiveWidget = RTHEXVISUALIZATION;
        break;

    case RTTABLEVISUALIZATION:
        previousActiveWidget = RTTABLEVISUALIZATION;
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

        hexVisualization->activity(true);
        activeWidget = HEXVISUALIZATION;

        if(!hexUpdated)
        {
            hexVisualization->update(project, logs->at(currentLogId).log);
            hexUpdated = true;
            return;
        }
        else
            hexVisualization->update();

        break;

    case TEXTVISUALIZATION:
        statusString += tr("Log size: ");
        statusString += QString::number(logs->at(currentLogId).log->size());
        statusString += tr(" records");
        labelTotalSize->setText(statusString);
        labelTotalSize->setVisible(true);
        actionGoToLine->setEnabled(true);
        stackedWidget->setCurrentWidget(textVisualization->getWidget());
        textVisualization->activity(true);
        activeWidget = TEXTVISUALIZATION;

        if(!textUpdated)
        {
            textVisualization->update(project, logs->at(currentLogId).log);
            textUpdated = true;
            return;
        }
        else
            textVisualization->update();

        break;

    case TABLEVISUALIZATION:
        statusString += tr("Log size: ");
        statusString += QString::number(logs->at(currentLogId).log->size());
        statusString += tr(" records");
        labelTotalSize->setText(statusString);
        labelTotalSize->setVisible(true);
        actionGoToLine->setEnabled(true);
        stackedWidget->setCurrentWidget(tableVisualization->getWidget());
        tableVisualization->activity(true);
        activeWidget = TABLEVISUALIZATION;

        if(!tableUpdated)
        {
            tableVisualization->update(project, logs->at(currentLogId).log);
            tableUpdated = true;
            return;
        }
        else
            tableVisualization->update();
        break;

    case RTTEXTVISUALIZATION:
        stackedWidget->setCurrentWidget(realTimeTextVisualization->getWidget());
        realTimeTextVisualization->update();
        activeWidget = RTTEXTVISUALIZATION;

        break;

    case RTHEXVISUALIZATION:
        stackedWidget->setCurrentWidget(realTimeHexVisualization->getWidget());
        realTimeHexVisualization->update();
        activeWidget = RTHEXVISUALIZATION;

        break;

    case RTTABLEVISUALIZATION:
        stackedWidget->setCurrentWidget(realTimeTableVisualization->getWidget());
        realTimeTableVisualization->update();
        activeWidget = RTTABLEVISUALIZATION;

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

        if(!textUpdated)
        {
            textVisualization->update(project, logs->at(currentLogId).log);
            textUpdated = true;
            return;
        }
        else
            textVisualization->update();

        break;
    case HEXVISUALIZATION:
        statusString += tr("Log size: ");
        statusString += QString::number(logs->at(currentLogId).log->size());
        statusString += tr(" records");
        labelTotalSize->setText(statusString);
        labelTotalSize->setVisible(true);
        actionGoToLine->setEnabled(true);
        stackedWidget->setCurrentWidget(hexVisualization->getWidget());

        if(!hexUpdated)
        {
            hexVisualization->update(project, logs->at(currentLogId).log);
            hexUpdated = true;
            return;
        }
        else
            hexVisualization->update();

        break;
    case TABLEVISUALIZATION:
        statusString += tr("Log size: ");
        statusString += QString::number(logs->at(currentLogId).log->size());
        statusString += tr(" records");
        labelTotalSize->setText(statusString);
        labelTotalSize->setVisible(true);
        actionGoToLine->setEnabled(true);
        stackedWidget->setCurrentWidget(tableVisualization->getWidget());

        if(!hexUpdated)
        {
            tableVisualization->update(project, logs->at(currentLogId).log);
            tableUpdated = true;
            return;
        }
        else
            tableVisualization->update();

        break;

    case RTTEXTVISUALIZATION:
        stackedWidget->setCurrentWidget(realTimeTextVisualization->getWidget());
        realTimeTextVisualization->update();
        break;

    case RTHEXVISUALIZATION:
        stackedWidget->setCurrentWidget(realTimeHexVisualization->getWidget());
        realTimeHexVisualization->update();
        break;

    case RTTABLEVISUALIZATION:
        stackedWidget->setCurrentWidget(realTimeTableVisualization->getWidget());
        realTimeTableVisualization->update();
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

void MainWindow::openConnection()
{
    QString errorString = QString::null;

    QString type = "";

    OpenConnectionDialog *openConnectionDialog = new OpenConnectionDialog();

    if(openConnectionDialog->exec())
    {
        type = openConnectionDialog->getConnectionType();
        delete openConnectionDialog;
    }
    else
    {
        delete openConnectionDialog;
        return;
    }

    IHostRealTimeSettings* rtSettings = StaticCoreUtils::getHostRealTimeSettings();

    // must be filled by QSettings
    QString projectFileName = rtSettings->projectPath(type);

    if(isProjectOpened)
        closeProject();

    QFile pFile(projectFileName);

    if(!pFile.exists())
    {
        errorMessager.showMessage(tr("Project file not found"));
        return;
    }

    project = new Project(projectFileName);
    project->load(errorString);

    if(!errorString.isNull())
    {
        errorMessager.showMessage(errorString);
        delete project;
        return;
    }

    QUdpSocket *socket = new QUdpSocket();
    socket->bind(QHostAddress(rtSettings->ip(type)), rtSettings->port(type), QUdpSocket::ShareAddress);

    socketAdapter = new UdpSocketAdapter(socket);

    actionClose->setEnabled(true);

    isProjectOpened = true;

    // TODO: enable all visualization actions here
    actionHexVisualization->setEnabled(true);
    actionTextVisualization->setEnabled(true);
    actionTableVisualization->setEnabled(true);

    realTime = true;

    realTimeTextVisualization->update(project, socketAdapter);
    realTimeHexVisualization->update(project, socketAdapter);
    realTimeTableVisualization->update(project, socketAdapter);

    if(settings.value("General/Gui/Default_visualization").value<QString>() == "hex")
        actionHexVisualization->toggle();
    else if(settings.value("General/Gui/Default_visualization").value<QString>() == "text")
        actionTextVisualization->toggle();
    else if(settings.value("General/Gui/Default_visualization").value<QString>() == "table")
        actionTableVisualization->toggle();

    setTitle(type, rtSettings->ip(type) + ":" + QString::number(rtSettings->port(type)));

    delete rtSettings;
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

    else
        openLog(project->logName(0));

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
        else if(activeWidget == TABLEVISUALIZATION)
            settings.setValue("General/Gui/Default_visualization", "table");

        actionClose->setEnabled(false);

        switchToWidget(EMPTY);

        closeLog();

        filtrationWidget->deactivate();

        delete project;

        isProjectOpened = false;

        menuCurrentLog->setEnabled(false);

        setWindowTitle(prName);
    }
}

void MainWindow::closeConnection()
{
    if(realTime)
    {
        if(activeWidget == RTHEXVISUALIZATION)
            settings.setValue("General/Gui/Default_visualization", "hex");
        else if(activeWidget == RTTEXTVISUALIZATION)
            settings.setValue("General/Gui/Default_visualization", "text");
        else if(activeWidget == RTTABLEVISUALIZATION)
            settings.setValue("General/Gui/Default_visualization", "table");

        actionClose->setEnabled(false);

        switchToWidget(EMPTY);

        delete project;
        delete socketAdapter;

        actionOpenConnection->setEnabled(true);

        realTime = false;
        isProjectOpened = false;

        actionHexVisualization->setEnabled(false);
        actionHexVisualization->setChecked(false);

        actionTextVisualization->setEnabled(false);
        actionTextVisualization->setChecked(false);

        actionTableVisualization->setEnabled(false);
        actionTableVisualization->setChecked(false);

        actionFiltration->setEnabled(false);

        hexUpdated = false;
        textUpdated = false;
        tableUpdated = false;

        setWindowTitle(prName);
    }
}

void MainWindow::closeAnything()
{
    if(realTime)
        closeConnection();
    else
        closeProject();
}

void MainWindow::openLog(QString name)
{
    QFileInfo projectFileInfo(project->projectName());
    QDir::setCurrent(projectFileInfo.absoluteDir().absolutePath());

    int eventsInfoSize = 0;
    info = project->info(eventsInfoSize);

    qint64 blockSize = settings.value("General/Core/Block_size").toLongLong();

    int memoryUsagePercent = settings.value("General/Core/Memory_usage").toInt();
    qint64 memoryUsage = OSTools::getTotalSystemMemory() * memoryUsagePercent / 100;

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

    logs->at(currentLogId).log->toggleActivity(true);

    setTitle(project->projectName(), logs->at(currentLogId).fileName);

    isLogOpened = true;

    menuCurrentLog->setEnabled(true);

    // TODO: enable all visualization actions here
    actionHexVisualization->setEnabled(true);
    actionTextVisualization->setEnabled(true);
    actionTableVisualization->setEnabled(true);

    actionFiltration->setEnabled(true);
    
    if(settings.value("General/Gui/Default_visualization").value<QString>() == "hex")
        actionHexVisualization->toggle();
    else if(settings.value("General/Gui/Default_visualization").value<QString>() == "text")
        actionTextVisualization->toggle();
    else if(settings.value("General/Gui/Default_visualization").value<QString>() == "table")
        actionTableVisualization->toggle();

    // TODO: добавить table visualization
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
        else if(actionTableVisualization->isChecked())
            actionTableVisualization->setChecked(false);
    
        if(!realTime)
            switchToWidget(TEXTVISUALIZATION);
        else
            switchToWidget(RTTEXTVISUALIZATION);
    }

    if(!checked && activeWidget == TEXTVISUALIZATION)
        switchToWidget(EMPTY);

    if(realTime)
    {
        if(!checked && activeWidget == RTTEXTVISUALIZATION)
            switchToWidget(EMPTY);
    }
}

void MainWindow::showHexVisualization(bool checked)
{
    if(checked)
    {
        if(actionTextVisualization->isChecked())
            actionTextVisualization->setChecked(false);
        else if(actionTableVisualization->isChecked())
            actionTableVisualization->setChecked(false);

        if(!realTime)
            switchToWidget(HEXVISUALIZATION);
        else
            switchToWidget(RTHEXVISUALIZATION);
    }

    if(!checked && activeWidget == HEXVISUALIZATION)
        switchToWidget(EMPTY);

    if(realTime)
    {
        if(!checked && activeWidget == RTHEXVISUALIZATION)
            switchToWidget(EMPTY);
    }
}

void MainWindow::showTableVisualization(bool checked)
{
    if(checked)
    {
        if(actionTextVisualization->isChecked())
            actionTextVisualization->setChecked(false);
        if(actionHexVisualization->isChecked())
            actionHexVisualization->setChecked(false);

        if(!realTime)
            switchToWidget(TABLEVISUALIZATION);
        else
            switchToWidget(RTTABLEVISUALIZATION);
    }

    if(!checked && activeWidget == TABLEVISUALIZATION)
        switchToWidget(EMPTY);

    if(realTime)
    {
        if(!checked && activeWidget == RTTABLEVISUALIZATION)
            switchToWidget(EMPTY);
    }

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
        else if(activeWidget == TABLEVISUALIZATION)
        {
            qint64 number = goToLineDialog->lineNumber();
            if(number < logs->at(currentLogId).log->size() || number >= 0)
                tableVisualization->fromLine(goToLineDialog->lineNumber());
        }
    }

    settings.setValue("Hidden/Gui/Line_dialog_pos", goToLineDialog->pos());
    settings.setValue("Hidden/Gui/Line_dialog_size", goToLineDialog->size());

    delete goToLineDialog;
}

void MainWindow::filteredLog(int id)
{
    currentLogId = id;
    updateActionsCurrentLogMenu();

    filtrationWidget->close();

    hexUpdated = false;
    textUpdated = false;
    tableUpdated = false;

    if(activeWidget == TEXTVISUALIZATION)
        updateVisualization(TEXTVISUALIZATION);
    else if(activeWidget == HEXVISUALIZATION)
        updateVisualization(HEXVISUALIZATION);
    else if(activeWidget == TABLEVISUALIZATION)
        updateVisualization(TABLEVISUALIZATION);

    setTitle(project->projectName(), logs->at(currentLogId).fileName);
}

/*void MainWindow::canceledFiltration()
{
    settings.setValue("Hidden/Gui/Filtration_dialog_pos", filtrationWidget->pos());

    filtrationWidget->close();
}*/

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

// ИЗМЕНЕНИЯ ТУТ
void MainWindow::switchCurrentLog()
{
    QAction *sender = (QAction*)QObject::sender();

    qDebug() << sender;

    for(int i = 0; i < logs->size(); i ++)
    {
        if(logs->at(i).fileName == sender->text())
        {
            // ПРОВЕРИТЬ НЕ ВЫБРАН ЛИ ТОТ ЖЕ ЛОГ
            logs->at(currentLogId).log->toggleActivity(false);

            currentLogId = i;

            logs->at(currentLogId).log->toggleActivity(true);

            WidgetType tempWidgetType = activeWidget;

            textUpdated = false;
            hexUpdated = false;
            tableUpdated = false;

            if(tempWidgetType == HEXVISUALIZATION || tempWidgetType == TEXTVISUALIZATION || tempWidgetType == TABLEVISUALIZATION)
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

void MainWindow::appliedSettings()
{
    if(activeWidget == HEXVISUALIZATION)
        updateVisualization(HEXVISUALIZATION);

    else if(activeWidget == TEXTVISUALIZATION)
        updateVisualization(TEXTVISUALIZATION);

    else if(activeWidget == TABLEVISUALIZATION)
        updateVisualization(TABLEVISUALIZATION);

    else if(activeWidget == RTTEXTVISUALIZATION)
        updateVisualization(RTTEXTVISUALIZATION);

    else if(activeWidget == RTHEXVISUALIZATION)
        updateVisualization(RTHEXVISUALIZATION);
}
