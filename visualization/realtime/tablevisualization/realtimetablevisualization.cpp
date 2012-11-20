#include "realtimetablevisualization.h"

RealTimeTableVisualization::RealTimeTableVisualization(QWidget *parent) :
    AbstractRealTimeTableVisualization(parent)
{
    recordsLimit = 1000;
    //recordsLimit = settings.value("RealTime/Text Visualization/Gui/RecordsLimit").value<quint64>();

    itemSearchedIndex = 0;
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

void RealTimeTableVisualization::searchNext(QString str)
{
    QList<QTableWidgetItem*> items = viewer->findItems(str, Qt::MatchContains | Qt::MatchCaseSensitive);

    if(items.size() == 0)
        return;

    if(!items[itemSearchedIndex]->isSelected())
    {
        viewer->setCurrentItem(items[itemSearchedIndex]);
        //items[itemSearchedIndex]->setSelected(true);
    }
    else
    {
        if(itemSearchedIndex + 1 < items.size())
        {
            itemSearchedIndex ++;
            searchNext(str);
        }
    }
}

void RealTimeTableVisualization::searchPrevious(QString str)
{
    QList<QTableWidgetItem*> items = viewer->findItems(str, Qt::MatchContains | Qt::MatchCaseSensitive);

    if(items.size() == 0)
        return;

    if(!items[itemSearchedIndex]->isSelected())
    {
        viewer->setCurrentItem(items[itemSearchedIndex]);
        //items[itemSearchedIndex]->setSelected(true);
    }
    else
    {
        if(itemSearchedIndex - 1 >= 0)
        {
            itemSearchedIndex --;
            searchNext(str);
        }
    }
}

QWidget *RealTimeTableVisualization::getWidget()
{
    return this;
}

void RealTimeTableVisualization::addRecord(QByteArray byteRecord)
{
    //if(recordsCount < recordsLimit)
    //{
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
    //}
}

RealTimeTableVisualization::~RealTimeTableVisualization()
{
}
