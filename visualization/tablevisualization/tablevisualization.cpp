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

void TableVisualization::update(IProject *project, ILog *log, QList<Format*> formats)
{
    this->formats = formats;

    currentProject = project;
    currentLog = log;

    topLinePos = 0;
    currentRow = 0;
    currentColumn = 0;

    viewer->clear();
    viewer->setRowCount( 0);

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

    eventTypes.sort();

    eventTypes.insert(0, "event");
    eventTypes.insert(0, "time");

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
                viewer->setItem(i, k, new QTableWidgetItem(QString::number(record.time)));
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

                        viewer->setItem(i, k, new QTableWidgetItem(
                                            updateValue(record.eventID,
                                                        j,
                                                        record.other[j],
                                                        info[record.eventID].argsInfo[j].type)
                                                        ));
                    }
                }

                if(!wasEvent)
                    //resultLine.append("");
                    viewer->setItem(i, k, new QTableWidgetItem(""));

            }
        }

        viewer->setRowHeight(i, 20);
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

    ui->horizontalScrollBar->setMinimum(viewer->horizontalScrollBar()->minimum());
    ui->horizontalScrollBar->setMaximum(viewer->horizontalScrollBar()->maximum());
}

void TableVisualization::updatePage(int cursorMoving)
{
    updatePage();
}

QString TableVisualization::updateValue(int eventID, int argumentID, QVariant value, LogDataType type)
{
    // OMG:

    QString result;

    if(formats.size() == 0)
    {
        if(type == BYTE_ARRAY_TYPE)
        {
            QString hexed_string = "";
            foreach(quint8 nextHex, value.value<QByteArray>())
            {
                QString hexed = QString::number(nextHex, 16);
                hexed = hexed.toUpper();
                if(hexed.size() == 1)
                    hexed.insert(0, '0');

                hexed_string += hexed + " ";
            }

            hexed_string.chop(1);

            result = hexed_string;
        }
        else
        {
            result = value.toString();
        }
    }
    else
    {
        foreach(Format *format, formats)
        {
            /*qDebug() << "---------";
            qDebug() << "type1" << type;
            qDebug() << "type2" << LogDataTypes::toLogDataType(format->formatInfo["argumentType"]);
            qDebug() << "eventID1" << eventID;
            qDebug() << "eventID2" << format->argument["eventID"].toInt();
            qDebug() << "argumentID1" << argumentID;
            qDebug() << "argumentID2" << format->argument["argumentID"].toInt();*/

            if(LogDataTypes::toLogDataType(format->formatInfo["argumentType"]) == type
                    && format->argument["eventID"].toInt() == eventID
                    && format->argument["argumentID"].toInt() == argumentID)
            {
                if(type == BYTE_ARRAY_TYPE)
                {
                    int length = 0;

                    if((format->formatInfo["unsizedField"].toInt() == 1))
                    {
                        length = formatLength(format);
                        length += value.value<QByteArray>().size() - length;
                    }
                    else
                    {
                        length = formatLength(format);
                    }

                    if(length != value.value<QByteArray>().size())
                    {
                        result += "Bad format(" + format->formatInfo["name"] + ") array: ";

                        QString hexed_string = "";
                        foreach(quint8 nextHex, value.value<QByteArray>())
                        {
                            QString hexed = QString::number(nextHex, 16);
                            hexed = hexed.toUpper();
                            if(hexed.size() == 1)
                                hexed.insert(0, '0');

                            hexed_string += hexed + " ";
                        }

                        hexed_string.chop(1);

                        result += hexed_string;
                    }
                    else
                    {
                        int iterator = 0;

                        foreach(FieldInfo field, format->fieldsInfo)
                        {
                            result += field["name"] + ": ";

                            int fieldSize = field["length"].toInt();

                            if(fieldSize == -1)
                                fieldSize = value.value<QByteArray>().size() - formatLength(format);

                            for(int i = 0; i < fieldSize; i ++)
                            {
                                QString hexed = QString::number((quint8)value.value<QByteArray>()[iterator], 16);
                                hexed = hexed.toUpper();
                                if(hexed.size() == 1)
                                    hexed.insert(0, '0');

                                result += hexed + " ";

                                iterator ++;
                            }
                        }
                    }
                }
                else
                {
                    result = value.toString();
                }
            }
            else if(type == BYTE_ARRAY_TYPE)
            {
                QString hexed_string = "";
                foreach(quint8 nextHex, value.value<QByteArray>())
                {
                    QString hexed = QString::number(nextHex, 16);
                    hexed = hexed.toUpper();
                    if(hexed.size() == 1)
                        hexed.insert(0, '0');

                    hexed_string += hexed + " ";
                }

                hexed_string.chop(1);

                result = hexed_string;
            }
            else
            {
                result = value.toString();
            }
        }
    }

    return result;
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
    eventTypes.insert(0, "time");

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

        resultLine.append(QString::number(record.time));
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
            header.append("time");

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

        resultLine.append(QString::number(record.time));

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

int TableVisualization::formatLength(Format *format)
{
    int result = 0;

    if(format->formatInfo["unsizedField"].toInt() != 1)
    {
        foreach(FieldInfo field, format->fieldsInfo)
        {
            result += field["length"].toInt();
        }
    }
    else
    {
        foreach(FieldInfo field, format->fieldsInfo)
        {
            if(field["length"].toInt() != -1)
                result += field["length"].toInt();
        }
    }

    return result;
}
