/*
 * textvisualization.cpp
 *
 *  Created on: May 8, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#include "textvisualization.h"
#include "ui_abstracttextvisualization.h"

TextVisualization::TextVisualization(QWidget *parent) :
    AbstractTextVisualization("Text visualization", parent)
{
    setSettings(settings);

    name = "Text";
}

void TextVisualization::activity(bool status)
{
    isActive = status;

    if(!isActive)
    {
        //ui->verticalScrollBar->setValue(0);
        //viewer->clear();
    }
}

void TextVisualization::update(IProject *project, ILog *log)
{
    viewer->setLineColor(settings.value("Text visualization/Appearance/Colors and Fonts/Cursor_line_color").value<QColor>());
    viewer->setLineFontColor(settings.value("Text visualization/Appearance/Colors and Fonts/Cursor_line_font_color").value<QColor>());

    viewer->setTextColor(settings.value("Text visualization/Appearance/Colors and Fonts/Main_text_foreground").value<QColor>());

    QPalette p = viewer->palette();
    p.setColor(QPalette::Base, settings.value("Text visualization/Appearance/Colors and Fonts/Main_text_background").value<QColor>());
    viewer->setPalette(p);

    viewer->setCurrentFont(settings.value("Text visualization/Appearance/Colors and Fonts/Font").value<QFont>());

    currentProject = project;
    currentLog = log;

    topLinePos = 0;

    viewer->clear();

    updatePage();
}

void TextVisualization::update()
{
    viewer->setLineColor(settings.value("Text visualization/Appearance/Colors and Fonts/Cursor_line_color").value<QColor>());
    viewer->setLineFontColor(settings.value("Text visualization/Appearance/Colors and Fonts/Cursor_line_font_color").value<QColor>());

    viewer->setTextColor(settings.value("Text visualization/Appearance/Colors and Fonts/Main_text_foreground").value<QColor>());

    QPalette p = viewer->palette();
    p.setColor(QPalette::Base, settings.value("Text visualization/Appearance/Colors and Fonts/Main_text_background").value<QColor>());
    viewer->setPalette(p);

    viewer->setCurrentFont(settings.value("Text visualization/Appearance/Colors and Fonts/Font").value<QFont>());

    int cursorMoving = currentLine - topLinePos;

    viewer->clear();    

    updatePage(cursorMoving);
}

QWidget *TextVisualization::getWidget()
{
    return this;
}

void TextVisualization::fromLine(qint64 line)
{
//    topLinePos = line;
//    updatePage();
    ui->verticalScrollBar->setValue(line);
}

TextVisualization::~TextVisualization()
{
    // delete colorsAndFontsSettings;
}

void TextVisualization::setSettings(QSettings &someSettings)
{
    if(!someSettings.contains("Defaults/Text visualization/Appearance/Colors and Fonts/Cursor_line_color"))
        someSettings.setValue("Defaults/Text visualization/Appearance/Colors and Fonts/Cursor_line_color", QColor(Qt::yellow).lighter(160));

    if(!someSettings.contains("Text visualization/Appearance/Colors and Fonts/Cursor_line_color"))
        someSettings.setValue("Text visualization/Appearance/Colors and Fonts/Cursor_line_color", QColor(Qt::yellow).lighter(160));

    if(!someSettings.contains("Defaults/Text visualization/Appearance/Colors and Fonts/Cursor_line_font_color"))
        someSettings.setValue("Defaults/Text visualization/Appearance/Colors and Fonts/Cursor_line_font_color", QColor(Qt::black));

    if(!someSettings.contains("Text visualization/Appearance/Colors and Fonts/Cursor_line_font_color"))
        someSettings.setValue("Text visualization/Appearance/Colors and Fonts/Cursor_line_font_color", QColor(Qt::black));         

    if(!someSettings.contains("Defaults/Text visualization/Appearance/Colors and Fonts/Main_text_background"))
        someSettings.setValue("Defaults/Text visualization/Appearance/Colors and Fonts/Main_text_background", QColor(Qt::white));

    if(!someSettings.contains("Text visualization/Appearance/Colors and Fonts/Main_text_background"))
        someSettings.setValue("Text visualization/Appearance/Colors and Fonts/Main_text_background", QColor(Qt::white));

    if(!someSettings.contains("Defaults/Text visualization/Appearance/Colors and Fonts/Main_text_foreground"))
        someSettings.setValue("Defaults/Text visualization/Appearance/Colors and Fonts/Main_text_foreground", QColor(Qt::black));

    if(!someSettings.contains("Text visualization/Appearance/Colors and Fonts/Main_text_foreground"))
        someSettings.setValue("Text visualization/Appearance/Colors and Fonts/Main_text_foreground", QColor(Qt::black));

    if(!someSettings.contains("Defaults/Text visualization/Appearance/Colors and Fonts/Font"))
        someSettings.setValue("Defaults/Text visualization/Appearance/Colors and Fonts/Font", QFont());

    if(!someSettings.contains("Text visualization/Appearance/Colors and Fonts/Font"))
        someSettings.setValue("Text visualization/Appearance/Colors and Fonts/Font", QFont());

    if(!someSettings.contains("Text visualization/Gui/Increment"))
        someSettings.setValue("Text visualization/Gui/Increment", 1);

    if(!someSettings.contains("Defaults/Text visualization/Gui/Increment"))
        someSettings.setValue("Defaults/Text visualization/Gui/Increment", 1);    
}

void TextVisualization::updatePage()
{
    currentLog->seek(topLinePos);
    viewer->clear();
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

    qint64 posInBinPage = 0;
    for(int i = 0; i < recordsCount; i ++)
    {
        qint64 readedSize = 0;
        Record record;
        int infoSize = 0;
        SimpleEventInfo *info = currentProject->info(infoSize);

        StaticRecordsReader::readRecord(binPage, binPageSize, posInBinPage, readedSize, record, info);
        posInBinPage += readedSize;

        QString resultLine = QString::number(topLinePos + i) + ": ";
        resultLine += "vTime: " + QString::number(record.vTime) + "; ";
        resultLine += "event: " + *info[record.eventID].type + "; ";

        for(int j = 0; j < info[record.eventID].argsCount; j ++)
        {
            if(info[record.eventID].argsInfo[j].type == BYTE_ARRAY_TYPE)
            {
                resultLine += *info[record.eventID].argsInfo[j].name + ": ";

                foreach(char nextHex, record.other[j].toByteArray())
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
    }

    ui->verticalScrollBar->setPageStep(recordsCount);
    ui->verticalScrollBar->setMaximum(currentLog->size() - recordsCount);

    if(direction == Up)
        viewer->moveCursor(QTextCursor::Start);
    else if(direction == Down)
    {
        viewer->moveCursor(QTextCursor::End);
        viewer->moveCursor(QTextCursor::StartOfLine);
    }

    ui->horizontalScrollBar->setMinimum(viewer->horizontalScrollBar()->minimum());
    ui->horizontalScrollBar->setMaximum(viewer->horizontalScrollBar()->maximum());
}

void TextVisualization::updatePage(int cursorMoving)
{
    updatePage();

    if(direction == Up)
    {
        for(int i = 0; i < cursorMoving; i++)
        {
            viewer->moveCursor(QTextCursor::StartOfLine);
            viewer->moveCursor(QTextCursor::Down);
        }

    }

    else if(direction == Down)
    {
        for(int i = linesOnPage() - 1; i > cursorMoving; i --)
        {
            viewer->moveCursor(QTextCursor::Up);
        }
    }
}
