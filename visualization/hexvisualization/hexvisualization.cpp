/*
 * hexvisualization.cpp
 *
 *  Created on: May 8, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#include "hexvisualization.h"
#include "ui_hexvisualization.h"

HexVisualization::HexVisualization(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HexVisualization)
{
    ui->setupUi(this);

    initObjectsConnections();

    ui->textEdit->installEventFilter(this);

    isActive = false;
}

void HexVisualization::activity(bool status)
{
    isActive = status;

    if(!isActive)
    {
        ui->verticalScrollBar->setValue(0);
        ui->textEdit->clear();
    }
}

void HexVisualization::update(IProject *project, ILog *log)
{
    currentProject = project;
    currentLog = log;

    topLinePos = 0;
    updatePage();
}

QWidget *HexVisualization::getWidget()
{
    return this;
}

void HexVisualization::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    if(isActive)
    {
        updatePage();
    }
}

bool HexVisualization::eventFilter(QObject *target, QEvent *event)
{
    if(event->type() == QEvent::Wheel && target == ui->textEdit)
        QCoreApplication::sendEvent(ui->verticalScrollBar, event);

    else if(event->type() == QEvent::KeyPress && target == ui->textEdit)
    {
        QKeyEvent *keyEvent = (QKeyEvent *)event;
        if(keyEvent->key() == Qt::Key_Up)
            ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderSingleStepSub);

        else if(keyEvent->key() == Qt::Key_Down)
            ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderSingleStepAdd);

        else if(keyEvent->key() == Qt::Key_PageUp)
            ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderPageStepSub);

        else if(keyEvent->key() == Qt::Key_PageDown)
            ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderPageStepAdd);

        else if(keyEvent->key() == Qt::Key_Home)
            ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderToMinimum);

        else if(keyEvent->key() == Qt::Key_End)
            ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderToMaximum);
    }

    return true;
}

HexVisualization::~HexVisualization()
{
    delete ui;
}

void HexVisualization::initObjectsConnections()
{
    QObject::connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarMoving(int)));
}

void HexVisualization::updatePage()
{
    currentLog->seek(topLinePos);
    ui->textEdit->clear();
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

        ui->textEdit->append(resultLine);
    }

    ui->verticalScrollBar->setPageStep(recordsCount);
    ui->verticalScrollBar->setMaximum(currentLog->size() - recordsCount);
}

int HexVisualization::linesOnPage()
{
    int wh = ui->textEdit->height();
    int fh = this->fontMetrics().height();
    return wh / fh - 1;
}

void HexVisualization::scrollBarMoving(int value)
{
    qDebug() << value;
    topLinePos = value;
    updatePage();
}
