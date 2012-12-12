#include "realtimetextvisualization.h"

RealTimeTextVisualization::RealTimeTextVisualization(QWidget *parent) :
    AbstractRealTimeTextVisualization(parent)
{
    recordsLimit = 1000;
    //recordsLimit = settings.value("RealTime/Text Visualization/Gui/RecordsLimit").value<quint64>();
}

void RealTimeTextVisualization::stop()
{
    disconnect(currentSocketAdapter, SIGNAL(dataRecieved(QByteArray)), this, SLOT(addRecord(QByteArray)));
}

void RealTimeTextVisualization::update(IProject *project, UdpSocketAdapter *socketAdapter, QList<Format*> formats)
{
    this->formats = formats;

    viewer->clear();
    recordsCount = 0;

    currentProject = project;
    currentSocketAdapter = socketAdapter;

    connect(currentSocketAdapter, SIGNAL(dataRecieved(QByteArray)), this, SLOT(addRecord(QByteArray)));
}

void RealTimeTextVisualization::update()
{
    if(settings.value("Text visualization/Gui/LineWrapMode").value<bool>())
        viewer->setLineWrapMode(QTextEdit::WidgetWidth);
    else
        viewer->setLineWrapMode(QTextEdit::NoWrap);

    viewer->setLineColor(settings.value("Text visualization/Appearance/Colors and Fonts/Cursor_line_color").value<QColor>());
    viewer->setLineFontColor(settings.value("Text visualization/Appearance/Colors and Fonts/Cursor_line_font_color").value<QColor>());

    viewer->setTextColor(settings.value("Text visualization/Appearance/Colors and Fonts/Main_text_foreground").value<QColor>());

    QPalette p = viewer->palette();
    p.setColor(QPalette::Base, settings.value("Text visualization/Appearance/Colors and Fonts/Main_text_background").value<QColor>());
    viewer->setPalette(p);

    viewer->setCurrentFont(settings.value("Text visualization/Appearance/Colors and Fonts/Font").value<QFont>());

    QString temp = viewer->toPlainText();
    viewer->clear();
    viewer->setText(temp);
}

void RealTimeTextVisualization::searchNext(QString str)
{
    viewer->find(str);
}

void RealTimeTextVisualization::searchPrevious(QString str)
{
    viewer->find(str, QTextDocument::FindBackward);
}

QWidget *RealTimeTextVisualization::getWidget()
{
    return this;
}

void RealTimeTextVisualization::addRecord(QByteArray byteRecord)
{
    //if(recordsCount < recordsLimit)
    //{
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
            resultLine += *info[record.eventID].argsInfo[j].name + ": ";

            resultLine += StaticVisualizationTools::updateValue(record.eventID,
                        j,
                        record.other[j],
                        info[record.eventID].argsInfo[j].type,
                                      formats);

            resultLine += "; ";
        }

        viewer->append(resultLine);

        recordsCount ++;
        viewer->moveCursor(QTextCursor::End);
        viewer->moveCursor(QTextCursor::StartOfLine);
    //}
    //else
    //{
    //    viewer->clear();
    //    recordsCount = 0;
    //}
}

RealTimeTextVisualization::~RealTimeTextVisualization()
{
}
