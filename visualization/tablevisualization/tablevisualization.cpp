#include "tablevisualization.h"

TableVisualization::TableVisualization(QWidget *parent)
    : AbstractTableVisualization(parent)
{
    setSettings(settings);
}

void TableVisualization::activity(bool status)
{
    isActive = status;
}

void TableVisualization::update(IProject *project, ILog *log)
{
    currentProject = project;
    currentLog = log;

    topLinePos = 0;
    currentRow = 0;
    currentColumn = 0;

    viewer->clear();

    eventTypes.clear();

    int size = 0;
    SimpleEventInfo *info = currentProject->info(size);

    for(int i = 0; i < size; i++)
    {
        int argsCount = info[i].argsCount;

        SimpleArgInfo *argsInfo = info[i].argsInfo;
        for(int j = 0; j < argsCount; j++)
        {
            if(!eventTypes.contains(*argsInfo[j].name))
                eventTypes.append(*argsInfo[j].name);
        }
    }

    eventTypes.insert(0, "event");
    eventTypes.insert(0, "vTime");

    viewer->setColumnCount(eventTypes.size());
    viewer->setHorizontalHeaderLabels(eventTypes);

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
    ui->verticalScrollBar->setValue(line);
}

void TableVisualization::updatePage()
{
    currentLog->seek(topLinePos);

    int recordsCount = linesOnPage();

    while(topLinePos > 0 && recordsCount > currentLog->size() - topLinePos)
    {
        topLinePos --;
        currentLog->seek(topLinePos);
    }

    if(topLinePos == 0 && recordsCount > currentLog->size())
    {
        recordsCount = currentLog->size();
    }

    qint64 binPageSize = 0;
    char *binPage = currentLog->read(recordsCount, binPageSize);

    viewer->setRowCount(recordsCount);

    qint64 posInBinPage = 0;
    for(int i = 0; i < recordsCount; i ++)
    {
        qint64 readedSize = 0;
        Record record;
        int infoSize = 0;
        SimpleEventInfo *info = currentProject->info(infoSize);

        StaticRecordsReader::readRecord(binPage, binPageSize, posInBinPage, readedSize, record, info);
        posInBinPage += readedSize;

        for(int k = 0; k < eventTypes.size(); k ++)
        {
            if(k == 0)
            {
                viewer->setItem(i, k, new QTableWidgetItem(QString::number(record.vTime)));
            }
            else if(k == 1)
            {
                viewer->setItem(i, k, new QTableWidgetItem(*info[record.eventID].type));
            }
            else
            {
                bool wasEvent = false;
                for(int j = 0; j < info[record.eventID].argsCount; j ++)
                {
                    if(eventTypes[k] == info[record.eventID].argsInfo[j].name)
                    {
                        wasEvent = true;

                        if(info[record.eventID].argsInfo[j].type == BYTE_ARRAY_TYPE)
                        {
                            QString hexed_string = "";
                            foreach(char nextHex, record.other[j].toByteArray())
                            {
                                QString hexed = QString::number(nextHex, 16);
                                hexed = hexed.toUpper();
                                if(hexed.size() == 1)
                                    hexed.insert(0, '0');

                                hexed_string += hexed + " ";
                            }

                            hexed_string.chop(1);

                            //resultLine.append(hexed_string);
                            viewer->setItem(i, k, new QTableWidgetItem(hexed_string));
                        }
                        else
                            //resultLine.append(record.other[j].toString());
                            viewer->setItem(i, k, new QTableWidgetItem(record.other[j].toString()));
                    }
                }

                if(!wasEvent)
                    //resultLine.append("");
                    viewer->setItem(i, k, new QTableWidgetItem(""));

            }
        }
    }

    ui->verticalScrollBar->setPageStep(recordsCount);
    ui->verticalScrollBar->setMaximum(currentLog->size() - recordsCount);

    QStringList header;
    for(int i = topLinePos; i < topLinePos + recordsCount; i ++)
    {
        header.append(QString::number(i));
    }

    viewer->setVerticalHeaderLabels(header);
    viewer->setCurrentCell(currentRow, currentColumn);

    viewer->resizeRowsToContents();

    //viewer->resizeRowsToContents();
    //viewer->resizeColumnsToContents();
    //viewer->resizeRowsToContents();

    ui->horizontalScrollBar->setMinimum(viewer->horizontalScrollBar()->minimum());
    ui->horizontalScrollBar->setMaximum(viewer->horizontalScrollBar()->maximum());
}

void TableVisualization::updatePage(int cursorMoving)
{
    updatePage();
}

/*void TableVisualization::allEventsUpdate(bool eventChanged)
{
    allEvents = true;

    if(eventChanged)
    {
        ui->recordsCountLabel->setText(QString::number(currentLog->size()));
    }

    int size = 0;
    SimpleEventInfo *info = currentProject->info(size);

    QStringList eventTypes;

    for(int i = 0; i < size; i++)
    {
        int argsCount = info[i].argsCount;

        SimpleArgInfo *argsInfo = info[i].argsInfo;
        for(int j = 0; j < argsCount; j++)
        {
            if(!eventTypes.contains(*argsInfo[j].name))
                eventTypes.append(*argsInfo[j].name);
        }
    }

    eventTypes.sort();
    eventTypes.insert(0, "event");
    eventTypes.insert(0, "vTime");

    viewer->setColumnCount(eventTypes.size());
    viewer->setHorizontalHeaderLabels(eventTypes);

    currentLog->seek(topLinePos);

    int recordsCount = linesOnPage();

    while(topLinePos > 0 && recordsCount > currentLog->size() - topLinePos)
    {
        topLinePos --;
        currentLog->seek(topLinePos);
    }

    if(topLinePos == 0 && recordsCount > currentLog->size())
    {
        recordsCount = currentLog->size();
    }

    qint64 binPageSize = 0;
    char *binPage = currentLog->read(recordsCount, binPageSize);

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
        resultLine.append(*info[record.eventID].type);

        for(int k = 2; k < eventTypes.size(); k ++)
        {
            bool wasEvent = false;
            for(int j = 0; j < info[record.eventID].argsCount; j ++)
            {
                if(eventTypes[k] == info[record.eventID].argsInfo[j].name)
                {
                    wasEvent = true;

                    if(info[record.eventID].argsInfo[j].type == BYTE_ARRAY_TYPE)
                    {
                        QString hexed_string = "";
                        foreach(char nextHex, record.other[j].toByteArray())
                        {
                            QString hexed = QString::number(nextHex, 16);
                            hexed = hexed.toUpper();
                            if(hexed.size() == 1)
                                hexed.insert(0, '0');

                            hexed_string += hexed + " ";
                        }

                        hexed_string.chop(1);

                        resultLine.append(hexed_string);
                    }
                    else
                        resultLine.append(record.other[j].toString());
                }
            }

            if(!wasEvent)
                resultLine.append("");
        }

        resultLines.append(resultLine);
    }

    viewer->setRowCount(recordsCount);

    for(int i = 0; i < resultLines.size(); i ++)
    {
        for(int j = 0; j < resultLines.at(i).size(); j ++)
        {
            viewer->setItem(i, j, new QTableWidgetItem(resultLines.at(i).at(j)));
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
    ui->verticalScrollBar->setMaximum(currentLog->size() - recordsCount);
}*/

/*void TableVisualization::eventsUpdate(bool eventChanged)
{
    allEvents = false;

    if(eventChanged)
    {
        currentEventLog = StaticFromLogSelector::selectFromLog(currentLog, currentProject, "tablev.templog", ui->toolBoxComboBox->currentIndex());
        ui->recordsCountLabel->setText(QString::number(currentEventLog->size()));
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
                QString hexed_string = "";
                foreach(char nextHex, record.other[j].toByteArray())
                {
                    QString hexed = QString::number(nextHex, 16);
                    hexed = hexed.toUpper();
                    if(hexed.size() == 1)
                        hexed.insert(0, '0');

                    hexed_string += hexed + " ";
                }

                hexed_string.chop(1);

                resultLine.append(hexed_string);
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
            //viewer->resizeRowToContents(i);
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
}*/

TableVisualization::~TableVisualization()
{
}

void TableVisualization::setSettings(QSettings &someSettings)
{
    if(!someSettings.contains("Defaults/Table visualization/Gui/Increment"))
        someSettings.setValue("Defaults/Table visualization/Gui/Increment", 1);

    if(!someSettings.contains("Table visualization/Gui/Increment"))
        someSettings.setValue("Table visualization/Gui/Increment", 1);
}
