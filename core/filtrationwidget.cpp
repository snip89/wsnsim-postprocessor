/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#include "filtrationwidget.h"
#include "ui_filtrationwidget.h"

FiltrationWidget::FiltrationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FiltrationWidget)
{
    ui->setupUi(this);

    addBooleanOperators();

    connect(ui->addFilterButton, SIGNAL(clicked()), this, SLOT(addFilter()));
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));

    ui->filtrationListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->filtrationListWidget, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showFiltrationListWidgetContextMenu(const QPoint&)));

    ui->logsListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->logsListWidget, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showLogsListWidgetContextMenu(const QPoint&)));

    logNameCounter = 0;
}

void FiltrationWidget::setCurrentProject(Project *project)
{
    currentProject = project;
}

void FiltrationWidget::setCurrentLog(int id)
{
    currentLogId = id;

    ui->currentLogLineEdit->setText(logs->at(currentLogId).fileName);
}

void FiltrationWidget::setMainLog(int id)
{
    mainLogId = id;

    ui->logsListWidget->addItem(logs->at(mainLogId).fileName);
}

void FiltrationWidget::setLogsInfos(QList<LogInfo> *logsInfos)
{
    logs = logsInfos;
}

void FiltrationWidget::activate()
{
    QStringList paramsNames = currentProject->paramsNames();
    foreach(QString paramName, paramsNames)
    {
        ui->filtrationParamComboBox->addItem(paramName);
    }
}

void FiltrationWidget::deactivate()
{
    ui->filtrationParamComboBox->clear();
    ui->filtrationListWidget->clear();

    filters.clear();

    ui->filtrationValueComboBox->clear();
}

void FiltrationWidget::clearLogs()
{
    ui->logsListWidget->clear();
}

FiltrationWidget::~FiltrationWidget()
{
    delete ui;
}

void FiltrationWidget::addBooleanOperators()
{
    ui->filtrationOperatorComboBox->addItem(tr("=="));
    ui->filtrationOperatorComboBox->addItem(tr("<"));
    ui->filtrationOperatorComboBox->addItem(tr("<="));
    ui->filtrationOperatorComboBox->addItem(tr(">"));
    ui->filtrationOperatorComboBox->addItem(tr(">="));
    ui->filtrationOperatorComboBox->addItem(tr("!="));
}

void FiltrationWidget::execute()
{
    if(!ui->filtrationListWidget->item(0))
    {
//        emit filtrationCanceled();
        emit logFiltered(currentLogId);
        return;
    }

    Log *currentLog = logs->at(currentLogId).log;

    int infoSize = 0;
    Log *tempLog = new Log(QString::number(logNameCounter) + ".templog", currentLog->blockSize, currentLog->memorySize, currentProject->info(infoSize), true);
    Log *newLog = new Log(QString::number(logNameCounter) + ".templog", currentLog->blockSize, currentLog->memorySize, currentProject->info(infoSize), true);

    for(int i = 0; i < filters.size(); i ++)
    {
//        qint64 fromPos = 0;

        if(i == 0)
        {
//            if(((Filter*)filters[i])->argumentName == "vTime")
//            {
//                if(((Filter*)filters[i])->booleanOperator == EQ ||
//                        ((Filter*)filters[i])->booleanOperator == GT ||
//                        ((Filter*)filters[i])->booleanOperator == GE)
//                {
//                    for(qint64 j = 0; j < currentLog->index->indexSize; j ++)
//                    {
//                        if((currentLog->index->index[j].vTime + currentLog->blockSize) >= ((Filter*)filters[i])->compareValue.toULongLong())
//                        {
//                            fromPos = currentLog->index->index[j].filePos;
//                            break;
//                        }
//                    }
//                }
//            }

            if(!StaticLogFilter::useFilter(currentLog, tempLog, filters[i]))
            {
                // FIXME: other logic need here
                errorMessager.showMessage(tr("Filtration error"));
                delete tempLog;
                delete newLog;
                return;
            }

            currentLog->toggleActivity(false);
            tempLog->toggleActivity(true);
        }
        else
        {
//            if(((Filter*)filters[i])->argumentName == "vTime")
//            {
//                if(((Filter*)filters[i])->booleanOperator == EQ ||
//                        ((Filter*)filters[i])->booleanOperator == GT ||
//                        ((Filter*)filters[i])->booleanOperator == GE)
//                {
//                    for(qint64 j = 0; j < tempLog->index->indexSize; j ++)
//                    {
//                        if((tempLog->index->index[j].vTime + tempLog->blockSize) >= ((Filter*)filters[i])->compareValue.toULongLong())
//                        {
//                            fromPos = tempLog->index->index[j].filePos;
//                            break;
//                        }
//                    }
//                }
//            }

            Log *tempLog2 = new Log(QString::number(logNameCounter) + ".templog", currentLog->blockSize, currentLog->memorySize, currentProject->info(infoSize), true);

            if(!StaticLogFilter::useFilter(tempLog, tempLog2, filters[i]))
            {
                // FIXME: other logic need here
                errorMessager.showMessage(tr("Filtration error"));
                return;
            }

            delete tempLog;
            tempLog = tempLog2;
            tempLog->toggleActivity(true);
        }
    }

    logNameCounter ++;
//    currentLogId ++;
    currentLogId = logs->size();

    newLog = tempLog;

    LogInfo newLogInfo;
    newLogInfo.id = currentLogId;
    newLogInfo.log = newLog;
    newLogInfo.fileName = newLog->fileName();

    QString toolTip;

    newLogInfo.filtersInfo = filtersExpressions();

    logs->append(newLogInfo);

    foreach(QString exp, newLogInfo.filtersInfo)
    {
        toolTip += exp + "\n";
    }
    toolTip.chop(1);

    QListWidgetItem *item = new QListWidgetItem(newLogInfo.fileName);
    item->setToolTip(toolTip);
    ui->logsListWidget->addItem(item);

    emit logFiltered(currentLogId);
}

QStringList FiltrationWidget::filtersExpressions()
{
    QStringList result;

    for(int i = 0; i < ui->filtrationListWidget->count(); i ++)
    {
        result.append(ui->filtrationListWidget->item(i)->text());
    }

    return result;
}

void FiltrationWidget::addFilter()
{
    if(ui->filtrationValueComboBox->text() == "")
        errorMessager.showMessage(tr("Value fild is empty"));

    else
    {
        BooleanOperators booleanOperator;

        if(ui->filtrationOperatorComboBox->currentText() == "==")
            booleanOperator = EQ;

        else if(ui->filtrationOperatorComboBox->currentText() == "<")
            booleanOperator = LT;

        else if(ui->filtrationOperatorComboBox->currentText() == "<=")
            booleanOperator = LE;

        else if(ui->filtrationOperatorComboBox->currentText() == ">")
            booleanOperator = GT;

        else if(ui->filtrationOperatorComboBox->currentText() == ">=")
            booleanOperator = GE;

        else if(ui->filtrationOperatorComboBox->currentText() == "!=")
            booleanOperator = NE;

        filters.append(new Filter(ui->filtrationParamComboBox->currentText(), booleanOperator, ui->filtrationValueComboBox->text()));

        QString filterInfoString;
        filterInfoString += ui->filtrationParamComboBox->currentText() + " ";
        filterInfoString += ui->filtrationOperatorComboBox->currentText() + " ";
        filterInfoString += ui->filtrationValueComboBox->text();

        ui->filtrationListWidget->addItem(filterInfoString);
    }
}

void FiltrationWidget::buttonClicked(QAbstractButton *button)
{
    if(button->text() == "OK")
    {
        execute();
    }
    else if(button->text() == "Cancel")
    {
        emit filtrationCanceled();
    }
}

void FiltrationWidget::showFiltrationListWidgetContextMenu(const QPoint& pos)
{
    QPoint globalPos = ui->filtrationListWidget->mapToGlobal(pos);

    QMenu contextMenu;
    contextMenu.addAction(tr("delete"));

    QListWidgetItem *item = ui->filtrationListWidget->itemAt(pos);

    if(item)
    {
        QAction* selectedItem = contextMenu.exec(globalPos);
        if(selectedItem)
        {
            filters.removeAt(ui->filtrationListWidget->row(item));
            delete item;
        }
    }
}

void FiltrationWidget::showLogsListWidgetContextMenu(const QPoint& pos)
{
    QPoint globalPos = ui->logsListWidget->mapToGlobal(pos);

    QMenu contextMenu;
    contextMenu.addAction(tr("set current"));
    contextMenu.addAction(tr("delete"));

    QListWidgetItem *item = ui->logsListWidget->itemAt(pos);

    if(item)
    {
        QAction* selectedItem = contextMenu.exec(globalPos);
        if(selectedItem)
        {
            
            if(selectedItem->text() == "set current")
            {
                QString fileName = item->text();

                for(int i = 0; i < logs->size(); i ++)
                {
                    if(logs->at(i).fileName == fileName)
                    {
                        logs->at(currentLogId).log->toggleActivity(false);

                        setCurrentLog(logs->at(i).id);

                        logs->at(currentLogId).log->toggleActivity(true);
                    }
                }

                // foreach(LogInfo logInfo, logs)
                // {
                //     if(logInfo.fileName == fileName)
                //     {
                //         setCurrentLog(logInfo.id);
                //     }
                // }

//            filters.removeAt(ui->filtrationListWidget->row(item));
//            delete item;
            }
            else if(selectedItem->text() == "delete")
            {
                QString fileName = item->text();

                for(int i = 0; i < logs->size(); i ++)
                {
                    if(logs->at(i).fileName == fileName)
                    {
                        if(i == mainLogId)
                            return;
                        else
                        {
                            qDebug() << "normal";

                            logs->at(currentLogId).log->toggleActivity(false);

                            Log *log = logs->at(currentLogId).log;

                            logs->removeAt(currentLogId);

                            delete log;

                            delete item;

                            setCurrentLog(mainLogId);

                            logs->at(currentLogId).log->toggleActivity(true);
                        }
                    }
                }
            }
        }
    }
}
