/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#include "hexvisualization.h"
#include "ui_abstracttextvisualization.h"

HexVisualization::HexVisualization(QWidget *parent) :
    AbstractTextVisualization("Hex visualization", parent)
{
    setSettings(settings);

    // colorsAndFontsSettings = new HexAppearanceColorsAndFontsSettings();
    // colorsAndFontsSettings->showCurrentSettings();
}

/*IVisualizationSettings *HexVisualization::visualizationSettings(QString name)
{
    if(name == tr("Colors and Fonts"))
    {
        return (IVisualizationSettings*)colorsAndFontsSettings;
    }

    return NULL;
}*/

void HexVisualization::activity(bool status)
{
    isActive = status;

    if(!isActive)
    {
        ui->verticalScrollBar->setValue(0);
        viewer->clear();
    }
}

void HexVisualization::update(IProject *project, ILog *log)
{
    viewer->setLineColor(settings.value("Hex visualization/Appearance/Colors and Fonts/Cursor_line_color").value<QColor>());
    viewer->setLineFontColor(settings.value("Hex visualization/Appearance/Colors and Fonts/Cursor_line_font_color").value<QColor>());

    viewer->setTextColor(settings.value("Hex visualization/Appearance/Colors and Fonts/Main_text_foreground").value<QColor>());

    QPalette p = viewer->palette();
    p.setColor(QPalette::Base, settings.value("Hex visualization/Appearance/Colors and Fonts/Main_text_background").value<QColor>());
    viewer->setPalette(p);

    viewer->setCurrentFont(settings.value("Hex visualization/Appearance/Colors and Fonts/Font").value<QFont>());

    currentProject = project;
    currentLog = log;

    topLinePos = 0;
    updatePage();
}

QWidget *HexVisualization::getWidget()
{
    return this;
}

void HexVisualization::fromLine(qint64 line)
{
//    topLinePos = line;
//    updatePage();
    ui->verticalScrollBar->setValue(line);
}

HexVisualization::~HexVisualization()
{
    // delete colorsAndFontsSettings;
}

void HexVisualization::setSettings(QSettings &someSettings)
{
    if(!someSettings.contains("Defaults/Hex visualization/Appearance/Colors and Fonts/Cursor_line_color"))
        someSettings.setValue("Defaults/Hex visualization/Appearance/Colors and Fonts/Cursor_line_color", QColor(Qt::yellow).lighter(160));

    if(!someSettings.contains("Hex visualization/Appearance/Colors and Fonts/Cursor_line_color"))
        someSettings.setValue("Hex visualization/Appearance/Colors and Fonts/Cursor_line_color", QColor(Qt::yellow).lighter(160));

    if(!someSettings.contains("Defaults/Hex visualization/Appearance/Colors and Fonts/Cursor_line_font_color"))
        someSettings.setValue("Defaults/Hex visualization/Appearance/Colors and Fonts/Cursor_line_font_color", QColor(Qt::black));

    if(!someSettings.contains("Hex visualization/Appearance/Colors and Fonts/Cursor_line_font_color"))
        someSettings.setValue("Hex visualization/Appearance/Colors and Fonts/Cursor_line_font_color", QColor(Qt::black));

    if(!someSettings.contains("Defaults/Hex visualization/Appearance/Colors and Fonts/Main_text_background"))
        someSettings.setValue("Defaults/Hex visualization/Appearance/Colors and Fonts/Main_text_background", QColor(Qt::white));

    if(!someSettings.contains("Hex visualization/Appearance/Colors and Fonts/Main_text_background"))
        someSettings.setValue("Hex visualization/Appearance/Colors and Fonts/Main_text_background", QColor(Qt::white));

    if(!someSettings.contains("Defaults/Hex visualization/Appearance/Colors and Fonts/Main_text_foreground"))
        someSettings.setValue("Defaults/Hex visualization/Appearance/Colors and Fonts/Main_text_foreground", QColor(Qt::black));

    if(!someSettings.contains("Hex visualization/Appearance/Colors and Fonts/Main_text_foreground"))
        someSettings.setValue("Hex visualization/Appearance/Colors and Fonts/Main_text_foreground", QColor(Qt::black));

    if(!someSettings.contains("Defaults/Hex visualization/Appearance/Colors and Fonts/Font"))
        someSettings.setValue("Defaults/Hex visualization/Appearance/Colors and Fonts/Font", QFont());

    if(!someSettings.contains("Hex visualization/Appearance/Colors and Fonts/Font"))
        someSettings.setValue("Hex visualization/Appearance/Colors and Fonts/Font", QFont());
}

void HexVisualization::updatePage()
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

        foreach(char nextHex, record.byteRecord)
        {
            QString hexed = QString::number((quint8)nextHex, 16);
            hexed = hexed.toUpper();
            if(hexed.size() == 1)
                hexed.insert(0, '0');

            resultLine += hexed + " ";
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
