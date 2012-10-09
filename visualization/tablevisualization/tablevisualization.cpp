#include "tablevisualization.h"

TableVisualization::TableVisualization(QWidget *parent)
    : AbstractTableVisualization(parent)
{
}

void TableVisualization::activity(bool status)
{
}

void TableVisualization::update(IProject *project, ILog *log)
{
    currentProject = project;
    currentLog = log;

    disconnect(ui->toolBoxComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(selectedEventChanged(QString)));
    initEventSelector();
    connect(ui->toolBoxComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(selectedEventChanged(QString)));

    topLinePos = 0;

    currentEvent = ui->toolBoxComboBox->currentText();

    updatePage();
}

void TableVisualization::update()
{
    updatePage();
}

QWidget *TableVisualization::getWidget()
{
    return this;
}

void TableVisualization::fromLine(qint64 line)
{
}

void TableVisualization::updatePage()
{
    int size = 0;
    SimpleEventInfo *info = currentProject->info(size);

    for(int i = 0; i < size; i++)
    {
        if(info[i].type == currentEvent)
        {
            int argsCount = info[i].argsCount;

            viewer->setColumnCount(argsCount + 1);

            QStringList header;
            header.append("vTime");

            SimpleArgInfo *argsInfo = info[i].argsInfo;
            for(int j = 0; j < argsCount; j++)
            {
                header.append(*argsInfo[j].name);
            }

            viewer->setHorizontalHeaderLabels(header);
        }
    }

    ui->horizontalScrollBar->setMinimum(viewer->horizontalScrollBar()->minimum());
    ui->horizontalScrollBar->setMaximum(viewer->horizontalScrollBar()->maximum());
}

void TableVisualization::updatePage(int cursorMoving)
{
    updatePage();
}

TableVisualization::~TableVisualization()
{
}

void TableVisualization::setSettings(QSettings &someSettings)
{
}

void TableVisualization::initEventSelector()
{
    int size = 0;
    SimpleEventInfo *info = currentProject->info(size);

    for(int i = 0; i < size; i ++)
    {
        ui->toolBoxComboBox->addItem(*info[i].type);
    }
}
