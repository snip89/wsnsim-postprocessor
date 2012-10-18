/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#include "filtrationwidget.h"
#include "ui_filtrationwidget.h"

FiltrationWidget::FiltrationWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FiltrationWidget)
{
    ui->setupUi(this);

    errorMessager.setModal(true);

    connect(ui->addFilterButton, SIGNAL(clicked()), this, SLOT(addFilter()));
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));

    ui->filtrationListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->filtrationListWidget, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showFiltrationListWidgetContextMenu(const QPoint&)));

    connect(this, SIGNAL(accepted()), this, SLOT(dialogIsAccepted()));
    connect(this, SIGNAL(rejected()), this, SLOT(dialogIsRejected()));

    connect(ui->filtrationParamComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(paramSelected(int)));

//    connect(ui->logsListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(selectedListItem(QListWidgetItem*)));

    logNameCounter = 0;
}

void FiltrationWidget::setCurrentProject(Project *project)
{
    currentProject = project;
}

void FiltrationWidget::setCurrentLog(int id)
{
    currentLogId = id;

//    ui->currentLogLineEdit->setText(logs->at(currentLogId).fileName);
}

void FiltrationWidget::setMainLog(int id)
{
    mainLogId = id;

    QListWidgetItem *item = new QListWidgetItem(logs->at(mainLogId).fileName);
    item->setToolTip(logs->at(mainLogId).filtersInfo.at(0));

//    ui->logsListWidget->addItem(item);
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

    updateBooleanOperators();
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
//    ui->logsListWidget->clear();
}

FiltrationWidget::~FiltrationWidget()
{
    delete ui;
}

void FiltrationWidget::updateBooleanOperators()
{
    ui->filtrationOperatorComboBox->clear();

    QString currentParamName = ui->filtrationParamComboBox->currentText();

    switch(currentProject->paramType(currentParamName))
    {
    case UNKNOWN_TYPE:
        break;

    case UINT8_TYPE:
        setIntegerBooleanOperators();
        break;

    case UINT16_TYPE:
        setIntegerBooleanOperators();
        break;

    case UINT32_TYPE:
        setIntegerBooleanOperators();
        break;

    case UINT64_TYPE:
        setIntegerBooleanOperators();
        break;

    case INT32_TYPE:
        setIntegerBooleanOperators();
        break;

    case BOOL_TYPE:
        setEqNeBooleanOperators();
        break;

    case DOUBLE_TYPE:
        setFloatBooleanOperators();
        break;

    case BYTE_ARRAY_TYPE:
        setEqNeBooleanOperators();
        break;

    case STRING_TYPE:
        setEqNeBooleanOperators();
        break;
    }
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

    QStringList currentFiltersInfo = logs->at(currentLogId).filtersInfo;

    int infoSize = 0;
    Log *tempLog = new Log(QString::number(logNameCounter) + ".templog", currentLog->blockSize, currentLog->memorySize, currentProject->info(infoSize), true);
    Log *newLog = new Log(QString::number(logNameCounter) + ".templog", currentLog->blockSize, currentLog->memorySize, currentProject->info(infoSize), true);

    for(int i = 0; i < filters.size(); i ++)
    {
//        qint64 fromPos = 0;

        if(i == 0)
        {
//            if(((Filter*)filters[i])->argumentName == "time")
//            {
//                if(((Filter*)filters[i])->booleanOperator == EQ ||
//                        ((Filter*)filters[i])->booleanOperator == GT ||
//                        ((Filter*)filters[i])->booleanOperator == GE)
//                {
//                    for(qint64 j = 0; j < currentLog->index->indexSize; j ++)
//                    {
//                        if((currentLog->index->index[j].time + currentLog->blockSize) >= ((Filter*)filters[i])->compareValue.toULongLong())
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
//            if(((Filter*)filters[i])->argumentName == "time")
//            {
//                if(((Filter*)filters[i])->booleanOperator == EQ ||
//                        ((Filter*)filters[i])->booleanOperator == GT ||
//                        ((Filter*)filters[i])->booleanOperator == GE)
//                {
//                    for(qint64 j = 0; j < tempLog->index->indexSize; j ++)
//                    {
//                        if((tempLog->index->index[j].time + tempLog->blockSize) >= ((Filter*)filters[i])->compareValue.toULongLong())
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

    newLogInfo.filtersInfo = currentFiltersInfo;

    foreach(QString filterInfo, filtersExpressions())
    {
        newLogInfo.filtersInfo.append(filterInfo);
    }

    logs->append(newLogInfo);

    foreach(QString exp, newLogInfo.filtersInfo)
    {
        toolTip += exp + "\n";
    }
    toolTip.chop(1);

    QListWidgetItem *item = new QListWidgetItem(newLogInfo.fileName);
    item->setToolTip(toolTip);
//    ui->logsListWidget->addItem(item);

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

void FiltrationWidget::setIntegerBooleanOperators()
{
    ui->filtrationOperatorComboBox->addItem(tr("=="));
    ui->filtrationOperatorComboBox->addItem(tr("<"));
    ui->filtrationOperatorComboBox->addItem(tr("<="));
    ui->filtrationOperatorComboBox->addItem(tr(">"));
    ui->filtrationOperatorComboBox->addItem(tr(">="));
    ui->filtrationOperatorComboBox->addItem(tr("!="));
}

void FiltrationWidget::setFloatBooleanOperators()
{
    ui->filtrationOperatorComboBox->addItem(tr("<"));
    ui->filtrationOperatorComboBox->addItem(tr(">"));
}

void FiltrationWidget::setEqNeBooleanOperators()
{
    ui->filtrationOperatorComboBox->addItem(tr("=="));
    ui->filtrationOperatorComboBox->addItem(tr("!="));
}

void FiltrationWidget::addFilter()
{
    if(ui->filtrationValueComboBox->text() == "")
        errorMessager.showMessage(tr("Value field is empty"));

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
    if(ui->buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole)
    {
        done(QDialog::Accepted);
    }

    else if(ui->buttonBox->buttonRole(button) == QDialogButtonBox::RejectRole)
    {
        done(QDialog::Rejected);
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

void FiltrationWidget::paramSelected(int index)
{
    int indexValue = index;

    qDebug() << "selected param: " << indexValue;

    updateBooleanOperators();
}

void FiltrationWidget::selectedListItem(QListWidgetItem *item)
{
    QString fileName = item->text();

    for(int i = 0; i < logs->size(); i ++)
    {
        if(logs->at(i).fileName == fileName)
        {
            logs->at(currentLogId).log->toggleActivity(false);

//                        setCurrentLog(logs->at(i).id);
            setCurrentLog(i);

            logs->at(currentLogId).log->toggleActivity(true);
        }
    }
}

void FiltrationWidget::dialogIsAccepted()
{
    execute();

    settings.setValue("Hidden/Gui/Filtration_dialog_pos", pos());
}

void FiltrationWidget::dialogIsRejected()
{
    settings.setValue("Hidden/Gui/Filtration_dialog_pos", pos());
}