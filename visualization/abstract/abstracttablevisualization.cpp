#include "abstracttablevisualization.h"

AbstractTableVisualization::AbstractTableVisualization(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::AbstractTableVisualization)
{
    ui->setupUi(this);

    connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarMoving(int)));

    isActive = false;

    viewer = new TableViewer();

    viewer->installEventFilter(this);

    ui->vAreaGridLayout->addWidget(viewer);
    connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), viewer->horizontalScrollBar(), SLOT(setValue(int)));
}

AbstractTableVisualization::~AbstractTableVisualization()
{
    delete viewer;

    delete ui;
}

void AbstractTableVisualization::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    if(isActive)
        updatePage();
}

bool AbstractTableVisualization::eventFilter(QObject *target, QEvent *event)
{
    if(event->type() == QEvent::Wheel && target == viewer)
    {

        int increment = settings.value("Table visualization/Gui/Increment").value<int>();

        if(((QWheelEvent*)event)->delta() < 0)
        {
            ui->verticalScrollBar->setValue(topLinePos + increment);
        }
        else
        {
            ui->verticalScrollBar->setValue(topLinePos - increment);
        }

        return true;
    }

    else if(event->type() == QEvent::KeyPress && target == viewer)
    {
        QKeyEvent *keyEvent = (QKeyEvent *)event;
        if(keyEvent->key() == Qt::Key_Up)
        {
            if(currentRow == 0)
            {
                ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderSingleStepSub);
            }
            else
            {
                currentRow --;
                viewer->setCurrentCell(currentRow, currentColumn);
            }

            return true;
        }

        else if(keyEvent->key() == Qt::Key_Left)
        {
            if(!currentColumn == 0)
            {
                currentColumn--;
                viewer->setCurrentCell(currentRow, currentColumn);
            }

            return true;
        }

        else if(keyEvent->key() == Qt::Key_Right)
        {
            if(!currentColumn == viewer->columnCount() - 1)
            {
                currentColumn ++;
                viewer->setCurrentCell(currentRow, currentColumn);
            }

            return true;
        }

        else if(keyEvent->key() == Qt::Key_Down)
        {
            if(currentRow == viewer->rowCount() - 1)
            {
                ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderSingleStepAdd);
            }
            else
            {
                currentRow ++;
                viewer->setCurrentCell(currentRow, currentColumn);
            }

            return true;
        }

        else if(keyEvent->key() == Qt::Key_PageUp)
        {
            currentRow = 0;
            ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderPageStepSub);

            return true;
        }

        else if(keyEvent->key() == Qt::Key_PageDown)
        {
            currentRow = viewer->rowCount() - 1;
            ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderPageStepAdd);
            viewer->setCurrentCell(currentRow, currentColumn);

            return true;
        }

        else if(keyEvent->key() == Qt::Key_Home)
        {
            currentRow = 0;
            ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderToMinimum);

            return true;
        }

        else if(keyEvent->key() == Qt::Key_End)
        {
            currentRow = viewer->rowCount() - 1;
            ui->verticalScrollBar->triggerAction(QAbstractSlider::SliderToMaximum);
            viewer->setCurrentCell(currentRow, currentColumn);

            return true;
        }
    }

    return false;
}

void AbstractTableVisualization::scrollBarMoving(int value)
{
    topLinePos = value;

    updatePage();
}

int AbstractTableVisualization::linesOnPage()
{
    viewer->setRowCount(1);
    viewer->setItem(0, 0, new QTableWidgetItem("test"));
    viewer->resizeRowToContents(0);

    return (viewer->viewport()->size().height() - TVIEWER_PADDING_KOEFF) / viewer->visualItemRect(viewer->item(0,0)).size().height();
}
