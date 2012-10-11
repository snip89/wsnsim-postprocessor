#include "tablevisualization.h"

TableVisualization::TableVisualization(QWidget *parent)
    : AbstractTableVisualization(parent)
{
    setSettings(settings);

    firstTime = true;
}

void TableVisualization::activity(bool status)
{
    isActive = status;
}

void TableVisualization::update(IProject *project, ILog *log)
{
    if(!firstTime)
    {
        qDebug() << "its here";
        delete currentEventLog;
    }
    else
        firstTime = false;

    currentProject = project;
    currentLog = log;

    disconnect(ui->toolBoxComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(selectedEventChanged(QString)));
    initEventSelector();
    connect(ui->toolBoxComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(selectedEventChanged(QString)));

    topLinePos = 0;

    currentRow = 0;
    currentColumn = 0;

    updatePage(true);
}

void TableVisualization::update()
{
    updatePage(true);
}

QWidget *TableVisualization::getWidget()
{
    return this;
}

void TableVisualization::fromLine(qint64 line)
{
    ui->verticalScrollBar->setValue(line);
}

void TableVisualization::updatePage(bool eventChanged)
{
    if(eventChanged)
    {
        currentRow = 0;
        currentColumn = 0;
        currentEvent = ui->toolBoxComboBox->currentText();
        currentEventLog = StaticFromLogSelector::selectFromLog(currentLog, currentProject, "tablev.templog", ui->toolBoxComboBox->currentIndex());
    }

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

    currentEventLog->seek(topLinePos);

    int recordsCount = linesOnPage();

    while(topLinePos > 0 && recordsCount > currentEventLog->size() - topLinePos)
    {
        topLinePos --;
        currentEventLog->seek(topLinePos);
    }

    if(topLinePos == 0 && recordsCount > currentEventLog->size())
    {
        recordsCount = currentEventLog->size();
    }

    qint64 binPageSize = 0;
    char *binPage = currentEventLog->read(recordsCount, binPageSize);

    QList<QStringList> resultLines;

    qint64 posInBinPage = 0;
    for(int i = 0; i < recordsCount; i ++)
    {
        QStringList resultLine;

        qint64 readedSize = 0;
        Record record;
        int infoSize = 0;
        SimpleEventInfo *info = currentProject->info(infoSize);

        StaticRecordsReader::readRecord(binPage, binPageSize, posInBinPage, readedSize, record, info);
        posInBinPage += readedSize;

        resultLine.append(QString::number(record.vTime));

        for(int j = 0; j < info[record.eventID].argsCount; j ++)
        {
            if(info[record.eventID].argsInfo[j].type == BYTE_ARRAY_TYPE)
            {
                foreach(char nextHex, record.other[j].toByteArray())
                {
                    QString hexed = QString::number(nextHex, 16);
                    hexed = hexed.toUpper();
                    if(hexed.size() == 1)
                        hexed.insert(0, '0');

                    resultLine.append(hexed);
                }
            }
            else
                resultLine.append(record.other[j].toString());
        }

        resultLines.append(resultLine);
    }

    viewer->setRowCount(recordsCount);

    for(int i = 0; i < resultLines.size(); i ++)
    {
        for(int j = 0; j < resultLines.at(i).size(); j ++)
        {
            viewer->setItem(i, j, new QTableWidgetItem(resultLines.at(i).at(j)));
            viewer->resizeRowToContents(i);
        }
    }

    QStringList header;
    for(int i = topLinePos; i < topLinePos + recordsCount; i ++)
    {
        header.append(QString::number(i));
    }

    viewer->setVerticalHeaderLabels(header);
    viewer->setCurrentCell(currentRow, currentColumn);

    ui->verticalScrollBar->setPageStep(recordsCount);
    ui->verticalScrollBar->setMaximum(currentEventLog->size() - recordsCount);

    ui->horizontalScrollBar->setMinimum(viewer->horizontalScrollBar()->minimum());
    ui->horizontalScrollBar->setMaximum(viewer->horizontalScrollBar()->maximum());
}

void TableVisualization::updatePage(int cursorMoving)
{
    updatePage(true);
}

TableVisualization::~TableVisualization()
{
    if(!firstTime)
        delete currentEventLog;
}

void TableVisualization::setSettings(QSettings &someSettings)
{
    if(!someSettings.contains("Defaults/Table visualization/Gui/Increment"))
        someSettings.setValue("Defaults/Table visualization/Gui/Increment", 1);

    if(!someSettings.contains("Table visualization/Gui/Increment"))
        someSettings.setValue("Table visualization/Gui/Increment", 1);
}

void TableVisualization::initEventSelector()
{
    ui->toolBoxComboBox->clear();

    int size = 0;
    SimpleEventInfo *info = currentProject->info(size);

    for(int i = 0; i < size; i ++)
    {
        ui->toolBoxComboBox->addItem(*info[i].type);
    }
}
