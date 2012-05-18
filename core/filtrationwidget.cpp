#include "filtrationwidget.h"

FiltrationWidget::FiltrationWidget(QWidget *parent)
: QWidget(parent)
{
    ui.setupUi(this);

    addBooleanOperators();

    connect(ui.addFilterButton, SIGNAL(clicked()), this, SLOT(addFilter()));
    connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));

    ui.filtrationListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.filtrationListWidget, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showFiltrationListWidgetContextMenu(const QPoint&)));
}

void FiltrationWidget::setCurrentLog(Log *log)
{
    currentLog = log;
}

void FiltrationWidget::setCurrentProject(Project *project)
{
    currentProject = project;
}

void FiltrationWidget::activate()
{
    QStringList paramsNames = currentProject->paramsNames();
    foreach(QString paramName, paramsNames)
    {
        ui.filtrationParamComboBox->addItem(paramName);
    }
}

void FiltrationWidget::deactivate()
{
    ui.filtrationParamComboBox->clear();
}

FiltrationWidget::~FiltrationWidget()
{

}

void FiltrationWidget::addBooleanOperators()
{
    ui.filtrationOperatorComboBox->addItem(tr("=="));
    ui.filtrationOperatorComboBox->addItem(tr("<"));
    ui.filtrationOperatorComboBox->addItem(tr("<="));
    ui.filtrationOperatorComboBox->addItem(tr(">"));
    ui.filtrationOperatorComboBox->addItem(tr(">="));
    ui.filtrationOperatorComboBox->addItem(tr("!="));
}

void FiltrationWidget::execute()
{
    if(!ui.filtrationListWidget->item(0))
    {
        emit filtrationCanceled();
        return;
    }

    int infoSize = 0;
    Log *tempLog = new Log("temp", currentLog->blockSize, currentLog->memorySize, currentProject->info(infoSize), true);
    Log *newLog = new Log("temp", currentLog->blockSize, currentLog->memorySize, currentProject->info(infoSize), true);

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

            Log *tempLog2 = new Log("temp", currentLog->blockSize, currentLog->memorySize, currentProject->info(infoSize), true);

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

    newLog = tempLog;

    emit logFiltered(newLog);
}

void FiltrationWidget::addFilter()
{
    if(ui.filtrationValueComboBox->text() == "")
        errorMessager.showMessage(tr("Value fild is empty"));

    else
    {
        BooleanOperators booleanOperator;

        if(ui.filtrationOperatorComboBox->currentText() == "==")
            booleanOperator = EQ;

        else if(ui.filtrationOperatorComboBox->currentText() == "<")
            booleanOperator = LT;

        else if(ui.filtrationOperatorComboBox->currentText() == "<=")
            booleanOperator = LE;

        else if(ui.filtrationOperatorComboBox->currentText() == ">")
            booleanOperator = GT;

        else if(ui.filtrationOperatorComboBox->currentText() == ">=")
            booleanOperator = GE;

        else if(ui.filtrationOperatorComboBox->currentText() == "!=")
            booleanOperator = NE;

        filters.append(new Filter(ui.filtrationParamComboBox->currentText(), booleanOperator, ui.filtrationValueComboBox->text()));

        QString filterInfoString;
        filterInfoString += ui.filtrationParamComboBox->currentText() + " ";
        filterInfoString += ui.filtrationOperatorComboBox->currentText() + " ";
        filterInfoString += ui.filtrationValueComboBox->text();

        ui.filtrationListWidget->addItem(filterInfoString);
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
    QPoint globalPos = ui.filtrationListWidget->mapToGlobal(pos);

    QMenu contextMenu;
    contextMenu.addAction(tr("delete"));

    QListWidgetItem *item = ui.filtrationListWidget->itemAt(pos);

    if(item)
    {
        QAction* selectedItem = contextMenu.exec(globalPos);
        if(selectedItem)
        {
            //            ui.filtrationListWidget->removeItemWidget(item);
            filters.removeAt(ui.filtrationListWidget->row(item));
            delete item;
        }
    }
}
