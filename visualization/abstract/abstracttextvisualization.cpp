#include "abstracttextvisualization.h"
#include "ui_abstracttextvisualization.h"

AbstractTextVisualization::AbstractTextVisualization(QString group, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbstractTextVisualization)
{
    ui->setupUi(this);

//    ui->horizontalScrollBar->setVisible(false);

    connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarMoving(int)));

    viewer = new Viewer(group);

    viewer->installEventFilter(this);

    ui->vAreaGridLayout->addWidget(viewer);
//    ui->gridLayout->addWidget(viewer, 0, 1);

    isActive = false;

    direction = Up;

    decrement = 0;

//    viewer->setHorizontalScrollBar(ui->horizontalScrollBar);
//    viewer->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), viewer->horizontalScrollBar(), SLOT(setValue(int)));

    currentLine = 0;

    connect(viewer, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPosChanging()));

//    connect(viewer->horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), ui->horizontalScrollBar, SLOT(hscrollBarRangeChanging(int, int)));

//    viewer->addScrollBarWidget(ui->horizontalScrollBar, Qt::AlignBottom);
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
    if(event->type() == QEvent::Wheel && target == viewer)
    {
        int increment = 0;

        if(name == "Hex")
        {
            increment = settings.value("Hex visualization/Gui/Increment").value<int>();
        }
        else if(name == "Text")
        {
            increment = settings.value("Text visualization/Gui/Increment").value<int>();
        }

        //increment = (currentLog->size() * increment) / 100;

        if(((QWheelEvent*)event)->delta() < 0)
            ui->verticalScrollBar->setValue(topLinePos + increment);
        else
            ui->verticalScrollBar->setValue(topLinePos - increment);
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

    updatePage();
}

void AbstractTextVisualization::cursorPosChanging()
{
    currentLine = topLinePos + viewer->textCursor().blockNumber();
}
