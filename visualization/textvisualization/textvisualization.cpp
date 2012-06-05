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
    colorsAndFontsSettings = new TextAppearanceColorsAndFontsSettings();
}

IVisualizationSettings *TextVisualization::visualizationSettings(QString name)
{
    if(name == "Colors and Fonts")
    {
        return (IVisualizationSettings*)colorsAndFontsSettings;
    }

    return NULL;
}

void TextVisualization::activity(bool status)
{
    isActive = status;

    if(!isActive)
    {
        ui->verticalScrollBar->setValue(0);
        viewer->clear();
    }
}

void TextVisualization::update(IProject *project, ILog *log)
{
    currentProject = project;
    currentLog = log;

    topLinePos = 0;
    updatePage();
}

QWidget *TextVisualization::getWidget()
{
    return this;
}

TextVisualization::~TextVisualization()
{
    delete colorsAndFontsSettings;
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
        viewer->moveCursor(QTextCursor::End);
}
