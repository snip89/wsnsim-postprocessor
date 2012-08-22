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

//    viewer->setHorizontalScrollBar(ui->horizontalScrollBar);
//    viewer->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), viewer->horizontalScrollBar(), SLOT(setValue(int)));

//    connect(viewer->horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), ui->horizontalScrollBar, SLOT(hscrollBarRangeChanging(int, int)));

//    viewer->addScrollBarWidget(ui->horizontalScrollBar, Qt::AlignBottom);
}

void AbstractTextVisualization::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    if(isActive)
    {
        updatePage();
    }
}

bool AbstractTextVisualization::eventFilter(QObject *target, QEvent *event)
{
    if(event->type() == QEvent::Wheel && target == viewer)
        QCoreApplication::sendEvent(ui->verticalScrollBar, event);

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
                viewer->moveCursor(QTextCursor::Up);
            }
        }

        else if(keyEvent->key() == Qt::Key_Down)
        {
            viewer->moveCursor(QTextCursor::EndOfLine);

            if(viewer->textCursor().atEnd())
            {
                viewer->moveCursor(QTextCursor::StartOfLine);
                direction = Down;
                ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderSingleStepAdd);
            }

            else
            {
                viewer->moveCursor(QTextCursor::StartOfLine);
                viewer->moveCursor(QTextCursor::Down);
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
        }

        else if(keyEvent->key() == Qt::Key_Home)
        {
            direction = Up;
            ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderToMinimum);
        }

        else if(keyEvent->key() == Qt::Key_End)
        {
            direction = Down;
            ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderToMaximum);
        }
    }

    return true;
}

AbstractTextVisualization::~AbstractTextVisualization()
{
    delete ui;
}

int AbstractTextVisualization::linesOnPage()
{
    int wh = viewer->height(); // - ui->verticalScrollBar->width();

    QFontMetrics m(viewer->currentFont());

    int fh = m.height();
//    int fh = m.lineWidth();

    return wh / fh;
}

void AbstractTextVisualization::scrollBarMoving(int value)
{
    qDebug() << value;
    topLinePos = value;
    updatePage();
}

/*void AbstractTextVisualization::hscrollBarRangeChanging(int min, int max)
{
    qDebug() << "hscroll range: " << min << " " << max;
    ui->horizontalScrollBar->setMinimum(min);
    ui->horizontalScrollBar->setMaximum(max);
}*/
