#include "realtimetablevisualization.h"

RealTimeTableVisualization::RealTimeTableVisualization(QWidget *parent) :
    AbstractRealTimeTableVisualization(parent)
{
    recordsLimit = 1000;
    //recordsLimit = settings.value("RealTime/Text Visualization/Gui/RecordsLimit").value<quint64>();
}

void RealTimeTableVisualization::stop()
{
    disconnect(currentSocketAdapter, SIGNAL(dataRecieved(QByteArray)), this, SLOT(addRecord(QByteArray)));
}

void RealTimeTableVisualization::update(IProject *project, UdpSocketAdapter *socketAdapter, QList<Format*> formats)
{
    this->formats = formats;

    viewer->clear();
    viewer->setRowCount(0);

    recordsCount = 0;

    currentProject = project;
    currentSocketAdapter = socketAdapter;

    argumentsNames.clear();

    QStringList argumentsNamesFull = settings.value("Hidden/Gui/Project/Columns_names").value<QStringList>();
    QStringList argumentsState = settings.value("Hidden/Gui/Project/Columns_state").value<QStringList>();

    argumentsNames = argumentsNamesFull;

    viewer->setColumnCount(argumentsNames.size());
    viewer->setHorizontalHeaderLabels(argumentsNames);

    for(int i = 0; i < argumentsNames.size(); i ++)
    {
        if(argumentsState[i] == "true")
            viewer->setColumnHidden(i, false);
        else
            viewer->setColumnHidden(i, true);
    }

    connect(currentSocketAdapter, SIGNAL(dataRecieved(QByteArray)), this, SLOT(addRecord(QByteArray)));
}

void RealTimeTableVisualization::update()
{
    argumentsNames.clear();

    QStringList argumentsNamesFull = settings.value("Hidden/Gui/Project/Columns_names").value<QStringList>();
    QStringList argumentsState = settings.value("Hidden/Gui/Project/Columns_state").value<QStringList>();

    argumentsNames = argumentsNamesFull;

    viewer->setColumnCount(argumentsNames.size());
    viewer->setHorizontalHeaderLabels(argumentsNames);

    for(int i = 0; i < argumentsNames.size(); i ++)
    {
        if(argumentsState[i] == "true")
            viewer->setColumnHidden(i, false);
        else
            viewer->setColumnHidden(i, true);
    }
}

QWidget *RealTimeTableVisualization::getWidget()
{
    return this;
}

void RealTimeTableVisualization::addRecord(QByteArray byteRecord)
{
    if(recordsCount < recordsLimit)
    {
        viewer->setRowCount(recordsCount + 1);

        qint64 readedSize = 0;
        Record record;
        int infoSize = 0;
        SimpleEventInfo *info = currentProject->info(infoSize);

        StaticRecordsReader::readRecord(byteRecord.data(), byteRecord.size(), 0, readedSize, record, info);

        for(int k = 0; k < argumentsNames.size(); k ++)
        {
            if(k == 0)
            {
                viewer->setItem(recordsCount, k, new QTableWidgetItem(QString::number(record.time)));
            }
            else if(k == 1)
            {
                viewer->setItem(recordsCount, k, new QTableWidgetItem(*info[record.eventID].type));
            }
            else
            {
                bool wasEvent = false;
                for(int j = 0; j < info[record.eventID].argsCount; j ++)
                {
                    if(argumentsNames[k] == info[record.eventID].argsInfo[j].name)
                    {
                        wasEvent = true;

                        viewer->setItem(recordsCount, k, new QTableWidgetItem(
                                            StaticVisualizationTools::updateValue(record.eventID,
                                                        j,
                                                        record.other[j],
                                                        info[record.eventID].argsInfo[j].type,
                                                                                  formats)
                                                        ));

                    }
                }

                if(!wasEvent)
                    //resultLine.append("");
                    viewer->setItem(recordsCount, k, new QTableWidgetItem(""));

            }
        }

        viewer->setRowHeight(recordsCount, 20);

        QStringList header;

        for(int i = 0; i < recordsCount + 1; i ++)
        {
            header.append(QString::number(i));
        }

        viewer->setVerticalHeaderLabels(header);
        //viewer->setCurrentCell(currentRow, currentColumn);

        recordsCount ++;
    }

    /*if(recordsCount < recordsLimit)
    {
        qint64 readedSize = 0;
        Record record;
        int infoSize = 0;
        SimpleEventInfo *info = currentProject->info(infoSize);

        StaticRecordsReader::readRecord(byteRecord.data(), byteRecord.size(), 0, readedSize, record, info);

        QString resultLine = QString::number(recordsCount) + ": ";
        resultLine += "time: " + QString::number(record.time) + "; ";
        resultLine += "event: " + *info[record.eventID].type + "; ";

        for(int j = 0; j < info[record.eventID].argsCount; j ++)
        {
            if(info[record.eventID].argsInfo[j].type == BYTE_ARRAY_TYPE)
            {
                resultLine += *info[record.eventID].argsInfo[j].name + ": ";

                foreach(quint8 nextHex, record.other[j].value<QByteArray>())
                {
                    QString hexed = QString::number(nextHex, 16);
                    hexed = hexed.toUpper();
                    if(hexed.size() == 1)
                        hexed.insert(0, '0');

                    resultLine += hexed + " ";
                }

                resultLine += "; ";
            }
            else
                resultLine += *info[record.eventID].argsInfo[j].name + ": " + record.other[j].toString() + "; ";
        }

        viewer->append(resultLine);

        recordsCount ++;
    }
    else
    {
        viewer->clear();
        recordsCount = 0;
    }*/
}

RealTimeTableVisualization::~RealTimeTableVisualization()
{
}
