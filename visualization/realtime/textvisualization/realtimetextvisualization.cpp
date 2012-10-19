#include "realtimetextvisualization.h"

RealTimeTextVisualization::RealTimeTextVisualization(QWidget *parent) :
    AbstractRealTimeTextVisualization(parent)
{
    recordsLimit = 1000;
    //recordsLimit = settings.value("RealTime/Text Visualization/Gui/RecordsLimit").value<quint64>();
}

void RealTimeTextVisualization::update(IProject *project, QUdpSocket *socket)
{
    viewer->clear();
    recordsCount = 0;

    currentProject = project;
    currentSocket = socket;
}

QWidget *RealTimeTextVisualization::getWidget()
{
    return this;
}

void RealTimeTextVisualization::addRecord(QByteArray byteRecord)
{
    if(recordsCount < recordsLimit)
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
    }
}

RealTimeTextVisualization::~RealTimeTextVisualization()
{
}
