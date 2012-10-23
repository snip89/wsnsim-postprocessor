#include "realtimehexvisualization.h"

RealTimeHexVisualization::RealTimeHexVisualization(QWidget *parent) :
    AbstractRealTimeTextVisualization(parent)
{
    recordsLimit = 1000;
}

void RealTimeHexVisualization::stop()
{
    disconnect(currentSocketAdapter, SIGNAL(dataRecieved(QByteArray)), this, SLOT(addRecord(QByteArray)));
}

void RealTimeHexVisualization::update(IProject *project, UdpSocketAdapter *socketAdapter)
{
    viewer->clear();
    recordsCount = 0;

    currentProject = project;
    currentSocketAdapter = socketAdapter;

    connect(currentSocketAdapter, SIGNAL(dataRecieved(QByteArray)), this, SLOT(addRecord(QByteArray)));
}

void RealTimeHexVisualization::update()
{
    if(settings.value("Hex visualization/Gui/LineWrapMode").value<bool>())
        viewer->setLineWrapMode(QTextEdit::WidgetWidth);
    else
        viewer->setLineWrapMode(QTextEdit::NoWrap);

    viewer->setLineColor(settings.value("Hex visualization/Appearance/Colors and Fonts/Cursor_line_color").value<QColor>());
    viewer->setLineFontColor(settings.value("Hex visualization/Appearance/Colors and Fonts/Cursor_line_font_color").value<QColor>());

    viewer->setTextColor(settings.value("Hex visualization/Appearance/Colors and Fonts/Main_text_foreground").value<QColor>());

    QPalette p = viewer->palette();
    p.setColor(QPalette::Base, settings.value("Hex visualization/Appearance/Colors and Fonts/Main_text_background").value<QColor>());
    viewer->setPalette(p);

    viewer->setCurrentFont(settings.value("Hex visualization/Appearance/Colors and Fonts/Font").value<QFont>());

    QString temp = viewer->toPlainText();
    viewer->clear();
    viewer->setText(temp);
}

QWidget *RealTimeHexVisualization::getWidget()
{
    return this;
}

void RealTimeHexVisualization::addRecord(QByteArray byteRecord)
{
    if(recordsCount < recordsLimit)
    {
        qint64 readedSize = 0;
        Record record;
        int infoSize = 0;
        SimpleEventInfo *info = currentProject->info(infoSize);

        StaticRecordsReader::readRecord(byteRecord.data(), byteRecord.size(), 0, readedSize, record, info);

        QString resultLine;

        foreach(char nextHex, record.byteRecord)
        {
            QString hexed = QString::number((quint8)nextHex, 16);
            hexed = hexed.toUpper();
            if(hexed.size() == 1)
                hexed.insert(0, '0');

            resultLine += hexed + " ";
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

RealTimeHexVisualization::~RealTimeHexVisualization()
{
}
