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

    rtHexUpdated = true;
    rtTextUpdated = true;
    rtTableUpdated = true;

    setSettings(settings);

    ui->setupUi(this);

    setWindowTitle(prName);

    realTime = false;

    createActions();
    createMenus();
    createStatusWidgets();

    initToolBar();

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

    setWindowIcon(QIcon(":/icons/base"));

    quickSearchWidget = new QuickSearchWidget();
    connect(quickSearchWidget, SIGNAL(findNext(QString)), this, SLOT(quickSearchNext(QString)));
    connect(quickSearchWidget, SIGNAL(findPrevious(QString)), this, SLOT(quickSearchPrevious(QString)));

    ui->searchToolBar->addWidget(quickSearchWidget);
    ui->searchToolBar->addAction(actionCloseSearch);
    ui->searchToolBar->setVisible(false);
}

void MainWindow::print()
{
    QPrinter printer;

    QTextDocument document;

    RecordsSelectionDialog recordsDialog(this);
    QPrintDialog printDialog(&printer, this);

    int fromRecord = 0;
    int toRecord = 0;

    if(!recordsDialog.exec())
    {
        return;
    }

    recordsDialog.getRecordsNumbers(fromRecord, toRecord);

    if(activeWidget == TEXTVISUALIZATION)
        textVisualization->getTextDocument(fromRecord, toRecord, document);

    else if(activeWidget == HEXVISUALIZATION)
        hexVisualization->getTextDocument(fromRecord, toRecord, document);

    else if(activeWidget == TABLEVISUALIZATION)
        tableVisualization->getTextDocument(fromRecord, toRecord, document);

    if(document.isEmpty())
        return;

    if (printDialog.exec() == QDialog::Accepted)
    {
        document.print(&printer);
    }
}

void MainWindow::exportAsTxt()
{
    QTextDocument document;

    QString name;

    RecordsSelectionDialog recordsDialog(this);

    int fromRecord = 0;
    int toRecord = 0;

    if(!recordsDialog.exec())
    {
        return;
    }

    recordsDialog.getRecordsNumbers(fromRecord, toRecord);

    if(activeWidget == TEXTVISUALIZATION)
        textVisualization->getTextDocument(fromRecord, toRecord, document);

    else if(activeWidget == HEXVISUALIZATION)
        hexVisualization->getTextDocument(fromRecord, toRecord, document);

    else if(activeWidget == TABLEVISUALIZATION)
        tableVisualization->getTextDocument(fromRecord, toRecord, document);

    if(document.isEmpty())
        return;

    QString dirPath = settings.value("General/Gui/ExportAsTxt_dialog_path").toString();

    QFileDialog *fileDialog = new QFileDialog(this, tr("Export as txt file"), dirPath, tr("txt files (*txt)"));
    fileDialog->resize(settings.value("Hidden/Gui/ExportAsTxt_dialog_size").value<QSize>());
    fileDialog->setWindowIcon(QIcon(":/icons/save_as"));

    fileDialog->setAcceptMode(QFileDialog::AcceptSave);

    if(fileDialog->exec())
        name = fileDialog->selectedFiles().at(0);

    settings.setValue("Hidden/Gui/ExportAsTxt_dialog_pos", fileDialog->pos());
    settings.setValue("Hidden/Gui/ExportAsTxt_dialog_size", fileDialog->size());

    delete fileDialog;

    QFile file(name);
    file.open(QFile::ReadWrite);

    QTextStream stream(&file);
    stream << document.toPlainText();

    file.close();
}

MainWindow::~MainWindow()
{
    closeAnything();

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

    /*foreach(Format *format, formats)
        delete format;*/

    delete quickSearchWidget;

    delete ui;
}

void MainWindow::setSettings(QSettings &someSettings)
{
    if(!someSettings.contains("Defaults/General/Gui/Recent_number"))
        someSettings.setValue("Defaults/General/Gui/Recent_number", DEFAULT_RECENT_NUMBER);

    if(!someSettings.contains("General/Gui/Recent_number"))
        someSettings.setValue("General/Gui/Recent_number", DEFAULT_RECENT_NUMBER);

    if(!someSettings.contains("Defaults/General/Gui/Project_file_dialog_path"))
        someSettings.setValue("Defaults/General/Gui/Project_file_dialog_path", QDir::homePath());

    if(!someSettings.contains("Defaults/General/Gui/ExportAsTxt_dialog_path"))
        someSettings.setValue("Defaults/General/Gui/ExportAsTxt_dialog_path", QDir::homePath());

    if(!someSettings.contains("General/Gui/Project_file_dialog_path"))
        someSettings.setValue("General/Gui/Project_file_dialog_path", QDir::homePath());

    if(!someSettings.contains("General/Gui/Format_file_dialog_path"))
        someSettings.setValue("General/Gui/Format_file_dialog_path", QDir::homePath());

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

    if(!someSettings.contains("Hidden/Gui/ExportAsTxt_dialog_size"))
        someSettings.setValue("Hidden/Gui/ExportAsTxt_dialog_size", QSize(320, 240));

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
        someSettings.setValue("Hidden/Gui/Color_dialog_size", QSize(320, 240));

    if(!someSettings.contains("Hidden/Gui/Color_dialog_pos"))
        someSettings.setValue("Hidden/Gui/Color_dialog_pos", QPoint(0, 0));

    if(!someSettings.contains("Hidden/Gui/Format_dialog_pos"))
        someSettings.setValue("Hidden/Gui/Format_dialog_pos", QPoint(0, 0));

    if(!someSettings.contains("Hidden/Gui/Columns_selection_dialog_pos"))
        someSettings.setValue("Hidden/Gui/Columns_selection_dialog_pos", QPoint(0, 0));
}

void MainWindow::createActions()
{
    actionOpen = new QAction(QIcon(":/icons/folder"), tr("&Open project..."), this);
    actionOpen->setShortcut(QKeySequence::Open);
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(openProject()));

    actionOpenConnection = new QAction(QIcon(":/icons/network"), tr("&Open connection..."), this);
    actionOpenConnection->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    connect(actionOpenConnection, SIGNAL(triggered()), this, SLOT(openConnection()));

    actionClose = new QAction(QIcon(":/icons/close_delete"), tr("&Close"), this);
    actionClose->setShortcut(QKeySequence::Close);
    actionClose->setEnabled(false);
    connect(actionClose, SIGNAL(triggered()), this, SLOT(closeAnything()));

    actionPrint = new QAction(QIcon(":/icons/printer"), tr("&Print..."), this);
    actionPrint->setShortcut(QKeySequence::Print);
    actionPrint->setEnabled(false);
    connect(actionPrint, SIGNAL(triggered()), this, SLOT(print()));

    actionExportAsTxt = new QAction(QIcon(":/icons/save_as"), tr("&Export as txt..."), this);
    actionExportAsTxt->setShortcut(QKeySequence::Save);
    actionExportAsTxt->setEnabled(false);
    connect(actionExportAsTxt, SIGNAL(triggered()), this, SLOT(exportAsTxt()));

    actionExit = new QAction(QIcon(":/icons/close_delete_2"), tr("&Exit"), this);
    actionExit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(exit()));

    actionSettings = new QAction(QIcon(":/icons/options"), tr("&Settings..."), this);
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

    actionQuickSearch = new QAction(QIcon(":/icons/search_lense"), tr("&Quick search"), this);
    actionQuickSearch->setShortcut(QKeySequence::Find);
    actionQuickSearch->setEnabled(false);
    connect(actionQuickSearch, SIGNAL(triggered()), this, SLOT(showQuickSearch()));

    actionGoToLine = new QAction(tr("&Go to line"), this);
    actionGoToLine->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    connect(actionGoToLine, SIGNAL(triggered()), this, SLOT(showGoToLineDialog()));
    actionGoToLine->setEnabled(false);

    //actionAcceptFormat = new QAction(tr("&Accept format..."), this);
    //actionAcceptFormat->setEnabled(false);
    //connect(actionAcceptFormat, SIGNAL(triggered()), this, SLOT(loadFormat()));

    //actionClearFormat = new QAction(tr("&Clear format"), this);
    //actionClearFormat->setEnabled(false);
    //connect(actionClearFormat, SIGNAL(triggered()), this, SLOT(clearFormat()));

    actionFormats = new QAction(QIcon(":/icons/tag_blue"), tr("&Formats..."), this);
    actionFormats->setEnabled(false);
    connect(actionFormats, SIGNAL(triggered()), this, SLOT(showFormatsDialog()));

    actionSelectColumns = new QAction(tr("&Select columns..."), this);
    actionSelectColumns->setEnabled(false);
    connect(actionSelectColumns, SIGNAL(triggered()), this, SLOT(showColumnsSelectionDialog()));

    actionFiltration = new QAction(QIcon(":/icons/refresh"), tr("&Filter log..."), this);
    actionFiltration->setEnabled(false);
    connect(actionFiltration, SIGNAL(triggered()), this, SLOT(showFiltration()));

    actionFullScreen = new QAction(tr("&Full screen"), this);
    actionFullScreen->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F11));
    actionFullScreen->setCheckable(true);
    connect(actionFullScreen, SIGNAL(toggled(bool)), this, SLOT(toggleFullScreen(bool)));

    actionHelp = new QAction(QIcon(":/icons/help"), tr("&Help"), this);

    //actionContextHelp = new QAction(tr("&Context help"), this);
    //actionContextHelp->setShortcut(QKeySequence::HelpContents);

    actionAbout = new QAction(QIcon(":/icons/information"), tr("&About..."), this);
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));

    actionAboutQt = new QAction(QIcon(":/icons/qt"), tr("&About Qt..."), this);
    connect(actionAboutQt, SIGNAL(triggered()), this, SLOT(showAboutQtDialog()));

    actionCloseSearch = new QAction(QIcon(":/icons/delete_2"), tr("&Close search"), this);
    connect(actionCloseSearch, SIGNAL(triggered()), this, SLOT(closeSearch()));
}

void MainWindow::showQuickSearch()
{
    ui->searchToolBar->setVisible(true);
}

void MainWindow::closeSearch()
{
    ui->searchToolBar->setVisible(false);
}

void MainWindow::initToolBar()
{
    ui->toolBar->addAction(actionOpen);
    ui->toolBar->addAction(actionOpenConnection);
    ui->toolBar->addAction(actionClose);

    ui->toolBar->addSeparator();

    ui->toolBar->addAction(actionExportAsTxt);
    ui->toolBar->addAction(actionPrint);

    //ui->toolBar->addSeparator();

    //ui->toolBar->addAction(actionAcceptFormat);
    //ui->toolBar->addAction(actionClearFormat);

    ui->toolBar->addSeparator();

    ui->toolBar->addAction(actionFormats);
    ui->toolBar->addAction(actionFiltration);

    ui->toolBar->addSeparator();

    ui->toolBar->addAction(actionSettings);
}

void MainWindow::createMenus()
{
    menuFile = new QMenu(tr("&File"), this);
    menuFile->addAction(actionOpen);
    menuFile->addAction(actionOpenConnection);

    //menuViewFiltration = new QMenu(tr("&Filtration"), this);

    //menuCurrentLog = new QMenu(tr("&Logs"), this);
    //menuCurrentLog->setEnabled(false);

    //menuViewFiltration->addMenu(menuCurrentLog);

    menuFiltrationLogs = new QMenu(tr("&Filtration (logs)"), this);
    menuFiltrationLogs->setEnabled(false);

    menuFile->addAction(actionClose);
    menuFile->addSeparator();

    menuRecentProjects = new QMenu(tr("&Recent projects..."), this);

    menuFile->addMenu(menuRecentProjects);
    menuFile->addSeparator();
    menuFile->addAction(actionExportAsTxt);
    menuFile->addAction(actionPrint);
    menuFile->addSeparator();
    menuFile->addAction(actionExit);

    menuEdit = new QMenu(tr("&Edit"), this);
    menuEdit->addAction(actionFormats);
    menuEdit->addAction(actionFiltration);
    menuEdit->addSeparator();

    menuEdit->addAction(actionQuickSearch);
    menuEdit->addAction(actionGoToLine);

    menuView = new QMenu(tr("&View"), this);
    menuView->addAction(actionHexVisualization);
    menuView->addAction(actionTextVisualization);
    menuView->addAction(actionTableVisualization);
    menuView->addSeparator();
    menuView->addAction(actionSelectColumns);
    menuView->addSeparator();
    //menuView->addMenu(menu);
    menuView->addMenu(menuFiltrationLogs);
    menuView->addSeparator();
    menuView->addAction(actionFullScreen);

    menuTools = new QMenu(tr("&Tools"), this);
    menuTools->addAction(actionSettings);

    menuHelp = new QMenu(tr("&Help"), this);
    menuHelp->addAction(actionHelp);
    //menuHelp->addAction(actionContextHelp);
    menuHelp->addSeparator();
    menuHelp->addAction(actionAbout);
    menuHelp->addAction(actionAboutQt);

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
    delete actionExportAsTxt;
    delete actionClose;
    delete actionExit;
    delete actionSettings;
    delete actionHexVisualization;
    delete actionTextVisualization;
    delete actionTableVisualization;
    //delete actionAcceptFormat;
    //delete actionClearFormat;
    delete actionSelectColumns;
    delete actionFiltration;
    delete actionFullScreen;
    delete actionQuickSearch;
    delete actionGoToLine;
    delete actionHelp;
    //delete actionContextHelp;
    delete actionAbout;
    delete actionAboutQt;
    delete actionCloseSearch;
}

void MainWindow::deleteMenus()
{
    delete menuFile;
    delete menuRecentProjects;
    //delete menuCurrentLog;
    delete menuEdit;
    delete menuFiltrationLogs;
    delete menuView;
    delete menuTools;
    delete menuHelp;
    //delete menuViewFiltration;
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
    foreach(QAction *action, menuFiltrationLogs->actions())
    {
        menuFiltrationLogs->removeAction(action);
        delete action;
    }

    for(int i = 0; i < logs->size(); i ++)
    {
        QAction *action = new QAction(logs->at(i).fileName, this);
        connect(action, SIGNAL(triggered()), this, SLOT(switchCurrentLog()));
        action->setCheckable(true);
        menuFiltrationLogs->addAction(action);

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

    actionFormats->setEnabled(false);
    //actionAcceptFormat->setEnabled(false);

    clearFormat();

    //actionClearFormat->setEnabled(false);
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
        actionSelectColumns->setEnabled(false);
        break;

    case RTTEXTVISUALIZATION:
        previousActiveWidget = RTTEXTVISUALIZATION;
        break;

    case RTHEXVISUALIZATION:
        previousActiveWidget = RTHEXVISUALIZATION;
        break;

    case RTTABLEVISUALIZATION:
        previousActiveWidget = RTTABLEVISUALIZATION;
        actionSelectColumns->setEnabled(false);
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
        actionQuickSearch->setEnabled(false);
        ui->searchToolBar->setVisible(false);
        actionPrint->setEnabled(false);
        actionExportAsTxt->setEnabled(false);
        break;

    case MAINSETTINGS:
//        stackedWidget->setCurrentWidget(mainSettings);
        activeWidget = MAINSETTINGS;
        break;

    case HEXVISUALIZATION:
        actionQuickSearch->setEnabled(true);

        statusString += tr("Log size: ");
        statusString += QString::number(logs->at(currentLogId).log->size());
        statusString += tr(" records");
        labelTotalSize->setText(statusString);
        labelTotalSize->setVisible(true);
        actionGoToLine->setEnabled(true);
        stackedWidget->setCurrentWidget(hexVisualization->getWidget());

        hexVisualization->activity(true);
        activeWidget = HEXVISUALIZATION;

        actionPrint->setEnabled(true);
        actionExportAsTxt->setEnabled(true);

        if(!hexUpdated)
        {
            hexVisualization->update(project, logs->at(currentLogId).log, formats);
            hexUpdated = true;
            return;
        }
        else
            hexVisualization->update(formats);

        break;

    case TEXTVISUALIZATION:
        actionQuickSearch->setEnabled(true);

        statusString += tr("Log size: ");
        statusString += QString::number(logs->at(currentLogId).log->size());
        statusString += tr(" records");
        labelTotalSize->setText(statusString);
        labelTotalSize->setVisible(true);
        actionGoToLine->setEnabled(true);
        stackedWidget->setCurrentWidget(textVisualization->getWidget());
        textVisualization->activity(true);
        activeWidget = TEXTVISUALIZATION;

        actionPrint->setEnabled(true);
        actionExportAsTxt->setEnabled(true);

        if(!textUpdated)
        {
            textVisualization->update(project, logs->at(currentLogId).log, formats);
            textUpdated = true;
            return;
        }
        else
            textVisualization->update(formats);

        break;

    case TABLEVISUALIZATION:
        actionQuickSearch->setEnabled(true);

        statusString += tr("Log size: ");
        statusString += QString::number(logs->at(currentLogId).log->size());
        statusString += tr(" records");
        labelTotalSize->setText(statusString);
        labelTotalSize->setVisible(true);
        actionGoToLine->setEnabled(true);
        stackedWidget->setCurrentWidget(tableVisualization->getWidget());
        tableVisualization->activity(true);
        activeWidget = TABLEVISUALIZATION;
        actionSelectColumns->setEnabled(true);

        actionPrint->setEnabled(true);
        actionExportAsTxt->setEnabled(true);

        if(!tableUpdated)
        {
            tableVisualization->update(project, logs->at(currentLogId).log, formats);
            tableUpdated = true;
            return;
        }
        else
            tableVisualization->update(formats);

        break;

    case RTTEXTVISUALIZATION:
        actionQuickSearch->setEnabled(true);

        stackedWidget->setCurrentWidget(realTimeTextVisualization->getWidget());
        activeWidget = RTTEXTVISUALIZATION;

        if(!rtTextUpdated)
        {
            realTimeTextVisualization->stop();
            realTimeTextVisualization->update(project, socketAdapter, formats);
            rtTextUpdated = true;
            return;
        }
        else
            realTimeTextVisualization->update();

        break;

    case RTHEXVISUALIZATION:
        actionQuickSearch->setEnabled(true);

        stackedWidget->setCurrentWidget(realTimeHexVisualization->getWidget());
        activeWidget = RTHEXVISUALIZATION;

        if(!rtHexUpdated)
        {
            realTimeHexVisualization->stop();
            realTimeHexVisualization->update(project, socketAdapter, formats);
            rtHexUpdated = true;
            return;
        }
        else
            realTimeHexVisualization->update();

        break;

    case RTTABLEVISUALIZATION:
        actionQuickSearch->setEnabled(true);

        stackedWidget->setCurrentWidget(realTimeTableVisualization->getWidget());
        activeWidget = RTTABLEVISUALIZATION;
        actionSelectColumns->setEnabled(true);

        if(!rtTableUpdated)
        {
            realTimeTableVisualization->stop();
            realTimeTableVisualization->update(project, socketAdapter, formats);
            rtTableUpdated = true;
            return;
        }
        else
            realTimeTableVisualization->update();

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
            textVisualization->update(project, logs->at(currentLogId).log, formats);
            textUpdated = true;
            return;
        }
        else
            textVisualization->update(formats);

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
            hexVisualization->update(project, logs->at(currentLogId).log, formats);
            hexUpdated = true;
            return;
        }
        else
            hexVisualization->update(formats);

        break;
    case TABLEVISUALIZATION:
        statusString += tr("Log size: ");
        statusString += QString::number(logs->at(currentLogId).log->size());
        statusString += tr(" records");
        labelTotalSize->setText(statusString);
        labelTotalSize->setVisible(true);
        actionGoToLine->setEnabled(true);
        stackedWidget->setCurrentWidget(tableVisualization->getWidget());

        if(!tableUpdated)
        {
            tableVisualization->update(project, logs->at(currentLogId).log, formats);
            tableUpdated = true;
            return;
        }
        else
            tableVisualization->update(formats);

        break;

    case RTTEXTVISUALIZATION:
        stackedWidget->setCurrentWidget(realTimeTextVisualization->getWidget());

        if(!rtTextUpdated)
        {
            realTimeTextVisualization->stop();
            realTimeTextVisualization->update(project, socketAdapter, formats);
            rtTextUpdated = true;
            return;
        }
        else
            realTimeTextVisualization->update();

        break;

    case RTHEXVISUALIZATION:
        stackedWidget->setCurrentWidget(realTimeHexVisualization->getWidget());

        if(!rtHexUpdated)
        {
            realTimeHexVisualization->stop();
            realTimeHexVisualization->update(project, socketAdapter, formats);
            rtHexUpdated = true;
            return;
        }
        else
            realTimeHexVisualization->update();

        break;

    case RTTABLEVISUALIZATION:
        stackedWidget->setCurrentWidget(realTimeTableVisualization->getWidget());

        if(!rtTableUpdated)
        {
            realTimeTableVisualization->stop();
            realTimeTableVisualization->update(project, socketAdapter, formats);
            rtTableUpdated = true;
            return;
        }
        else
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

void MainWindow::showFormatsDialog()
{
    FormatsDialog *dialog = new FormatsDialog(project, formats);

    dialog->exec();

    formats = dialog->getFormats();

    delete dialog;

    if(!realTime)
    {
        if(activeWidget == TEXTVISUALIZATION)
            updateVisualization(TEXTVISUALIZATION);

        if(activeWidget == HEXVISUALIZATION)
            updateVisualization(HEXVISUALIZATION);

        if(activeWidget == TABLEVISUALIZATION)
            updateVisualization(TABLEVISUALIZATION);
    }
    else
    {
        rtHexUpdated = false;
        rtTextUpdated = false;
        rtTableUpdated = false;

        if(activeWidget == RTTEXTVISUALIZATION)
            updateVisualization(RTTEXTVISUALIZATION);

        if(activeWidget == RTHEXVISUALIZATION)
            updateVisualization(RTHEXVISUALIZATION);

        if(activeWidget == RTTABLEVISUALIZATION)
            updateVisualization(RTTABLEVISUALIZATION);
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

void MainWindow::showAboutDialog()
{
    //if(!settings.contains("Localization/Language"))

    QString language = settings.value("Localization/Language").value<QString>();

    QString resName;

    if(language == "Ru")
        resName = ":/other/about_ru";

    if(language == "En")
        resName = ":/other/about_en";

    QFile file(resName);

    file.open(QFile::ReadOnly);

    QTextStream stream(&file);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    stream.setCodec(codec);

    QMessageBox::about(this, tr("About"), stream.readAll());
}

void MainWindow::showAboutQtDialog()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::clearFormat()
{
    if(realTime)
    {
        realTimeTextVisualization->stop();
        realTimeHexVisualization->stop();
        realTimeTableVisualization->stop();
    }

    foreach(Format *format, formats)
        delete format;

    formats.clear();

    //actionClearFormat->setEnabled(false);

    if(!realTime)
    {
        hexUpdated = false;
        textUpdated = false;
        tableUpdated = false;

        if(activeWidget == TEXTVISUALIZATION)
            updateVisualization(TEXTVISUALIZATION);

        if(activeWidget == HEXVISUALIZATION)
            updateVisualization(HEXVISUALIZATION);

        if(activeWidget == TABLEVISUALIZATION)
            updateVisualization(TABLEVISUALIZATION);
    }
    else
    {
        rtHexUpdated = false;
        rtTextUpdated = false;
        rtTableUpdated = false;

        if(activeWidget == RTTEXTVISUALIZATION)
            updateVisualization(RTTEXTVISUALIZATION);

        if(activeWidget == RTHEXVISUALIZATION)
            updateVisualization(RTHEXVISUALIZATION);

        if(activeWidget == RTTABLEVISUALIZATION)
            updateVisualization(RTTABLEVISUALIZATION);
    }
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
    openConnectionDialog->setWindowIcon(QIcon(":/icons/network"));

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

    int eventsInfoSize = 0;
    info = project->info(eventsInfoSize);

    if(project->projectParams.visualizationInfo.columnsSettings == "")
    {
        QStringList columnsNames = StaticVisualizationTools::argumentsNames(info, eventsInfoSize);
        QStringList columnsState;

        for(int i = 0; i < columnsNames.size(); i++)
        {
            columnsState.append("true");
        }

        settings.setValue("Hidden/Gui/Project/Columns_names", columnsNames);
        settings.setValue("Hidden/Gui/Project/Columns_state", columnsState);
    }
    else
    {
        QStringList columnsSettings = project->projectParams.visualizationInfo.columnsSettings.split(';');
        QStringList columnsNames = columnsSettings[0].split(',');
        QStringList columnsState = columnsSettings[1].split(',');

        settings.setValue("Hidden/Gui/Project/Columns_names", columnsNames);
        settings.setValue("Hidden/Gui/Project/Columns_state", columnsState);
    }

    if(project->projectParams.visualizationInfo.formatsSettings != "")
    {
        QStringList formatsSettings = project->projectParams.visualizationInfo.formatsSettings.split(';');

        if(formatsSettings.size() != 0)
        {
            foreach(QString formatInfo, formatsSettings)
            {
                loadFormat(formatInfo);
            }
        }
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

    actionFormats->setEnabled(true);
    //actionAcceptFormat->setEnabled(true);

    realTime = true;

    realTimeTextVisualization->update(project, socketAdapter, formats);
    realTimeHexVisualization->update(project, socketAdapter, formats);
    realTimeTableVisualization->update(project, socketAdapter, formats);

    if(settings.value("General/Gui/Default_visualization").value<QString>() == "hex")
        actionHexVisualization->toggle();
    else if(settings.value("General/Gui/Default_visualization").value<QString>() == "text")
        actionTextVisualization->toggle();
    else if(settings.value("General/Gui/Default_visualization").value<QString>() == "table")
        actionTableVisualization->toggle();

    setTitle(type, rtSettings->ip(type) + ":" + QString::number(rtSettings->port(type)));

    delete rtSettings;
}

void MainWindow::loadFormat(QString formatInfo)
{
    QString errorString = QString::null;

    QStringList formatInfos = formatInfo.split(',');
    QString name = formatInfos[3];

    QDir dir;
    QFileInfo finfo = QFileInfo(name);

    QString dirPath = dir.filePath(name);
    dirPath.chop(finfo.fileName().size());

    QDir::setCurrent(QApplication::applicationDirPath());

    QLibrary formatDataLibrary("./formatData");
    if(!formatDataLibrary.load())
    {
        errorMessager.showMessage(tr("Error while loading formatData library"));
        return;
    }

    typedef Format*(*formatDataLoad) (QString& formatFileName, QString* errorMessage);
    formatDataLoad load = (formatDataLoad) formatDataLibrary.resolve("load");

    Format *format = load(name, &errorString);

    FormatValidator::validate(format, errorString);

    if(!errorString.isNull())
    {
        errorMessager.showMessage(errorString);
        return;
    }

    QDir::setCurrent(dirPath);

    QFile luaFile(format->luaInfo["file"]);
    if(!luaFile.exists())
    {
        errorMessager.showMessage(tr("Lua file not found: ") + format->luaInfo["file"]);
        return;
    }

    QDir::setCurrent(QApplication::applicationDirPath());

    AttrInfo info;
    info["eventID"] = formatInfos[0];
    info["argumentID"] = formatInfos[1];
    info["eventType"] = formatInfos[2];

    format->argument = info;

    formats.append(format);
}

void MainWindow::openProject(QString name)
{
    settings.setValue("Hidden/Core/Current_pos", 0);

    if(name == QString::null)
    {
        QString dirPath = settings.value("General/Gui/Project_file_dialog_path").toString();

        QFileDialog *fileDialog = new QFileDialog(this, tr("Open project file"), dirPath, tr("XML project files (*xml)"));
        fileDialog->move(settings.value("Hidden/Gui/File_dialog_pos").value<QPoint>());
        fileDialog->resize(settings.value("Hidden/Gui/File_dialog_size").value<QSize>());
        fileDialog->setWindowIcon(QIcon(":/icons/folder"));
        
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
        settings.setValue("General/Gui/Project_file_dialog_path", dirPath);

        if(realTime)
            closeConnection();

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

        if(realTime)
            closeConnection();

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

    int eventsInfoSize = 0;
    info = project->info(eventsInfoSize);

    if(project->projectParams.visualizationInfo.columnsSettings == "")
    {
        QStringList columnsNames = StaticVisualizationTools::argumentsNames(info, eventsInfoSize);
        QStringList columnsState;

        for(int i = 0; i < columnsNames.size(); i++)
        {
            columnsState.append("true");
        }

        settings.setValue("Hidden/Gui/Project/Columns_names", columnsNames);
        settings.setValue("Hidden/Gui/Project/Columns_state", columnsState);
    }
    else
    {
        QStringList columnsSettings = project->projectParams.visualizationInfo.columnsSettings.split(';');
        QStringList columnsNames = columnsSettings[0].split(',');
        QStringList columnsState = columnsSettings[1].split(',');

        settings.setValue("Hidden/Gui/Project/Columns_names", columnsNames);
        settings.setValue("Hidden/Gui/Project/Columns_state", columnsState);
    }

    if(project->projectParams.visualizationInfo.formatsSettings != "")
    {
        QStringList formatsSettings = project->projectParams.visualizationInfo.formatsSettings.split(';');

        if(formatsSettings.size() != 0)
        {
            foreach(QString formatInfo, formatsSettings)
            {
                loadFormat(formatInfo);
            }
        }
    }

    setTitle(project->projectName(), QString::null);

    removeActionsRecent();
    insertToRecent(name);
    insertActionsRecent();

    int count = project->logsCount();

    if(count == 0)
    {
        errorMessager.showMessage(tr("No log file in project"));
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

        QString existingIndexInfo = project->projectParams.indexFileInfo["name"];

        QDir dir;
        QFileInfo finfo = QFileInfo(project->projectName());

        QString dirPath = dir.filePath(project->projectName());
        dirPath.chop(finfo.fileName().size());
        QDir::setCurrent(dirPath);

        if(settings.value("General/Core/Save_index").value<bool>())
        {
            if(existingIndexInfo != "")
            {
                QFile indexFile(existingIndexInfo);
                if(!indexFile.exists())
                {
                    QString indexFileName = logs->at(0).log->saveIndex(project->projectName());

                    IndexFileInfo indexFileInfo;
                    indexFileInfo["name"] = indexFileName;

                    project->projectParams.indexFileInfo = indexFileInfo;
                }
                else
                {
                    indexFile.open(QFile::ReadOnly);

                    QDataStream stream(&indexFile);

                    QString realFileName;
                    stream >> realFileName;

                    if(project->projectName() != realFileName)
                    {
                        indexFile.close();

                        indexFile.remove();

                        QString indexFileName = logs->at(0).log->saveIndex(project->projectName());

                        IndexFileInfo indexFileInfo;
                        indexFileInfo["name"] = indexFileName;

                        project->projectParams.indexFileInfo = indexFileInfo;
                    }

                    indexFile.close();
                }
            }
            else
            {
                QString indexFileName = logs->at(0).log->saveIndex(project->projectName());

                IndexFileInfo indexFileInfo;
                indexFileInfo["ID"] = "0";
                indexFileInfo["name"] = indexFileName;

                project->projectParams.indexFileInfo = indexFileInfo;
            }
        }

        closeLog();

        filtrationWidget->deactivate();

        QString errorString;

        QString columnsSettings = "";

        foreach(QString str, settings.value("Hidden/Gui/Project/Columns_names").value<QStringList>())
        {
            columnsSettings += str + ",";
        }

        columnsSettings.chop(1);

        columnsSettings += ";";

        foreach(QString str, settings.value("Hidden/Gui/Project/Columns_state").value<QStringList>())
        {
            columnsSettings += str + ",";
        }

        columnsSettings.chop(1);

        project->projectParams.visualizationInfo.columnsSettings = columnsSettings;
        project->save(errorString);

        if(!errorString.isNull())
        {
            errorMessager.showMessage(errorString);
            return;
        }

        delete project;

        isProjectOpened = false;

        menuFiltrationLogs->setEnabled(false);

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

        actionHexVisualization->setEnabled(false);
        actionHexVisualization->setChecked(false);

        actionTextVisualization->setEnabled(false);
        actionTextVisualization->setChecked(false);

        actionTableVisualization->setEnabled(false);
        actionTableVisualization->setChecked(false);

        actionFiltration->setEnabled(false);

        switchToWidget(EMPTY);

        QString errorString;

        QString columnsSettings = "";

        foreach(QString str, settings.value("Hidden/Gui/Project/Columns_names").value<QStringList>())
        {
            columnsSettings += str + ",";
        }

        columnsSettings.chop(1);

        columnsSettings += ";";

        foreach(QString str, settings.value("Hidden/Gui/Project/Columns_state").value<QStringList>())
        {
            columnsSettings += str + ",";
        }

        columnsSettings.chop(1);

        project->projectParams.visualizationInfo.columnsSettings = columnsSettings;
        project->save(errorString);

        if(!errorString.isNull())
        {
            errorMessager.showMessage(errorString);
            return;
        }

        delete project;
        delete socketAdapter;

        actionOpenConnection->setEnabled(true);

        realTime = false;
        isProjectOpened = false;

        rtHexUpdated = true;
        rtTextUpdated = true;
        rtTableUpdated = true;

        setWindowTitle(prName);

        actionFormats->setEnabled(false);
        //actionAcceptFormat->setEnabled(false);

        clearFormat();

        //actionClearFormat->setEnabled(false);
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

    QDir dir;
    QFileInfo finfo = QFileInfo(project->projectName());

    QString dirPath = dir.filePath(project->projectName());
    dirPath.chop(finfo.fileName().size());
    QDir::setCurrent(dirPath);

    QFile logIndexFile(project->projectParams.indexFileInfo["name"]);

    if(settings.value("General/Core/Save_index").value<bool>() && logIndexFile.exists())
    {
        if(!logs->at(0).log->load(false, false))
        {
            closeLog();
            return;
        }

        if(!logs->at(0).log->loadIndex(project->projectParams.indexFileInfo["name"], project->projectName()))
        {
            if(!logs->at(0).log->load(true, false))
            {
                closeLog();
                return;
            }
        }
    }
    else
    {
        if(!logs->at(0).log->load(true, false))
        {
            closeLog();
            return;
        }
    }

    filtrationWidget->setMainLog(0);

    updateActionsCurrentLogMenu();

    logs->at(0).log->toggleActivity(true);

    setTitle(project->projectName(), logs->at(0).fileName);

    isLogOpened = true;

    menuFiltrationLogs->setEnabled(true);

    // TODO: enable all visualization actions here
    actionHexVisualization->setEnabled(true);
    actionTextVisualization->setEnabled(true);
    actionTableVisualization->setEnabled(true);

    actionFormats->setEnabled(true);
    //actionAcceptFormat->setEnabled(true);

    actionFiltration->setEnabled(true);
    
    if(settings.value("General/Gui/Default_visualization").value<QString>() == "hex")
        actionHexVisualization->toggle();
    else if(settings.value("General/Gui/Default_visualization").value<QString>() == "text")
        actionTextVisualization->toggle();
    else if(settings.value("General/Gui/Default_visualization").value<QString>() == "table")
        actionTableVisualization->toggle();
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
    /*if(currentLogId == id)
    {
        currentLogId = id;
        updateActionsCurrentLogMenu();
    }*/

    currentLogId = id;
    updateActionsCurrentLogMenu();

    filtrationWidget->close();

    hexUpdated = false;
    textUpdated = false;
    tableUpdated = false;

    settings.setValue("Hidden/Core/Current_pos", 0);

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

// Ð˜Ð—ÐœÐ•ÐÐ•ÐÐ˜Ð¯ Ð¢Ð£Ð¢
void MainWindow::switchCurrentLog()
{
    QAction *sender = (QAction*)QObject::sender();

    qDebug() << sender;

    for(int i = 0; i < logs->size(); i ++)
    {
        if(logs->at(i).fileName == sender->text())
        {
            // ÐŸÐ ÐžÐ’Ð•Ð Ð˜Ð¢Ð¬ ÐÐ• Ð’Ð«Ð‘Ð ÐÐ Ð›Ð˜ Ð¢ÐžÐ¢ Ð–Ð• Ð›ÐžÐ“
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

    foreach(QAction *action, menuFiltrationLogs->actions())
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

void MainWindow::showColumnsSelectionDialog()
{
    ColumnsSelectionDialog *dialog = new ColumnsSelectionDialog();

    dialog->move(settings.value("Hidden/Gui/Format_dialog_pos").value<QPoint>());

    if(dialog->exec())
    {
        if(activeWidget == TABLEVISUALIZATION)
            updateVisualization(TABLEVISUALIZATION);

        if(activeWidget == RTTABLEVISUALIZATION)
            updateVisualization(RTTABLEVISUALIZATION);
    }

    settings.setValue("Hidden/Gui/Format_dialog_pos", dialog->pos());

    delete dialog;
}

void MainWindow::quickSearchNext(QString str)
{
    if(str != "")
    {
        if(!realTime)
        {
            if(activeWidget == HEXVISUALIZATION)
                hexVisualization->searchNext(str);

            if(activeWidget == TEXTVISUALIZATION)
                textVisualization->searchNext(str);

            if(activeWidget == TABLEVISUALIZATION)
                tableVisualization->searchNext(str);
        }
        else
        {
            if(activeWidget == RTHEXVISUALIZATION)
                realTimeHexVisualization->searchNext(str);

            if(activeWidget == RTTEXTVISUALIZATION)
                realTimeTextVisualization->searchNext(str);

            if(activeWidget == RTTABLEVISUALIZATION)
                realTimeTableVisualization->searchNext(str);
        }
    }
}

void MainWindow::quickSearchPrevious(QString str)
{
    if(str != "")
    {
        if(!realTime)
        {
            if(activeWidget == HEXVISUALIZATION)
                hexVisualization->searchPrevious(str);

            if(activeWidget == TEXTVISUALIZATION)
                textVisualization->searchPrevious(str);

            if(activeWidget == TABLEVISUALIZATION)
                tableVisualization->searchPrevious(str);
        }
        else
        {
            if(activeWidget == RTHEXVISUALIZATION)
                realTimeHexVisualization->searchPrevious(str);

            if(activeWidget == RTTEXTVISUALIZATION)
                realTimeTextVisualization->searchPrevious(str);

            if(activeWidget == RTTABLEVISUALIZATION)
                realTimeTableVisualization->searchPrevious(str);
        }
    }
}
