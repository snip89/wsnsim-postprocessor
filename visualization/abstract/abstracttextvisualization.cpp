#include "abstracttextvisualization.h"
#include "ui_abstracttextvisualization.h"

AbstractTextVisualization::AbstractTextVisualization(QString group, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbstractTextVisualization)
{
    ui->setupUi(this);

    connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarMoving(int)));

    viewer = new Viewer(group);
    viewer->installEventFilter(this);

    ui->vAreaGridLayout->addWidget(viewer);

    isActive = false;

    direction = Up;
    decrement = 0;
    currentLine = 0;

    wheel = false;

    connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), viewer->horizontalScrollBar(), SLOT(setValue(int)));
    connect(viewer, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPosChanging()));
}

void AbstractTextVisualization::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    if(isActive)
    {
        int cursorMoving = currentLine - topLinePos;

        updatePage(cursorMoving);
    }
}

bool AbstractTextVisualization::eventFilter(QObject *target, QEvent *event)
{
    qDebug() << "currentLine: " << currentLine;

    if(event->type() == QEvent::Wheel && target == viewer)
    {
        wheel = true;

        int increment = 0;

        if(name == "Hex")
        {
            increment = settings.value("Hex visualization/Gui/Increment").value<int>();
        }
        else if(name == "Text")
        {
            increment = settings.value("Text visualization/Gui/Increment").value<int>();
        }

        if(((QWheelEvent*)event)->delta() < 0)
        {
            /*if(currentLine < (topLinePos + increment))
                viewer->setInvisible(true);

            if(currentLine > (topLinePos + increment))
                viewer->setInvisible(false);*/

            ui->verticalScrollBar->setValue(topLinePos + increment);
        }
        else
        {
            /*if(currentLine > (topLinePos - increment + linesOnPage(decrement) - 1))
                viewer->setInvisible(true);*/

            ui->verticalScrollBar->setValue(topLinePos - increment);
        }
    }

    else if(event->type() == QEvent::KeyPress && target == viewer)
    {
        QKeyEvent *keyEvent = (QKeyEvent *)event;
        if(keyEvent->key() == Qt::Key_Up)
        {
            viewer->moveCursor(QTextCursor::StartOfLine);

            if(viewer->textCursor().atStart())
            {
                direction = Up;
                ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderSingleStepSub);
            }

            else
            {
                viewer->moveCursor(QTextCursor::PreviousBlock);
            }
        }

        else if(keyEvent->key() == Qt::Key_Down)
        {
            viewer->moveCursor(QTextCursor::EndOfBlock);

            if(viewer->textCursor().atEnd())
            {
                viewer->moveCursor(QTextCursor::StartOfLine);
                direction = Down;
                ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderSingleStepAdd);
                viewer->moveCursor(QTextCursor::StartOfBlock);
            }

            else
            {
                viewer->moveCursor(QTextCursor::NextBlock);
            }
        }

        else if(keyEvent->key() == Qt::Key_PageUp)
        {
            direction = Up;
            ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderPageStepSub);
        }

        else if(keyEvent->key() == Qt::Key_PageDown)
        {
            direction = Down;
            ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderPageStepAdd);
            viewer->moveCursor(QTextCursor::StartOfBlock);
        }

        else if(keyEvent->key() == Qt::Key_Home)
        {
            direction = Up;
            ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderToMinimum);

            viewer->moveCursor(QTextCursor::Start);
        }

        else if(keyEvent->key() == Qt::Key_End)
        {
            direction = Down;
            ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderToMaximum);

            viewer->moveCursor(QTextCursor::End);
            viewer->moveCursor(QTextCursor::StartOfBlock);
        }
    }

    return true;
}

AbstractTextVisualization::~AbstractTextVisualization()
{
    delete ui;
}

int AbstractTextVisualization::linesOnPage(int decrement)
{
    QFontMetricsF m(viewer->currentFont());

    int wh = viewer->viewport()->height() - VIEWER_PADDING_KOEFF;

    int fh = m.lineSpacing();

    int lines = wh / fh;

    return lines - decrement;
}


void AbstractTextVisualization::scrollBarMoving(int value)
{
    topLinePos = value;

    int cursorMoving = 0;

    if(wheel)
    {
        cursorMoving = currentLine - topLinePos;
        updatePage(cursorMoving);
        wheel = false;
    }
    else
    {
        updatePage();
    }
}

void AbstractTextVisualization::cursorPosChanging()
{
    currentLine = topLinePos + viewer->textCursor().blockNumber();
}
