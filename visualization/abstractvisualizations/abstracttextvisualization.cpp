#include "abstracttextvisualization.h"
#include "ui_abstracttextvisualization.h"

AbstractTextVisualization::AbstractTextVisualization(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbstractTextVisualization)
{
    ui->setupUi(this);

    connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarMoving(int)));

    ui->textEdit->installEventFilter(this);

    viewer = new Viewer();

    ui->gridLayout->addWidget(viewer, 0, 0);

    isActive = false;
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

AbstractTextVisualization::~AbstractTextVisualization()
{
    delete ui;
}

int AbstractTextVisualization::linesOnPage()
{
    int wh = viewer->height();
    int fh = this->fontMetrics().height();
    return wh / fh - 1;
}

void AbstractTextVisualization::scrollBarMoving(int value)
{
    qDebug() << value;
    topLinePos = value;
    updatePage();
}
