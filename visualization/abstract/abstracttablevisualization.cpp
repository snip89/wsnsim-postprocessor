#include "abstracttablevisualization.h"

AbstractTableVisualization::AbstractTableVisualization(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::AbstractTableVisualization)
{
    ui->setupUi(this);

    connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarMoving(int)));

    isActive = false;

    viewer = new TableViewer();
    ui->vAreaGridLayout->addWidget(viewer);
    connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), viewer->horizontalScrollBar(), SLOT(setValue(int)));
}

AbstractTableVisualization::~AbstractTableVisualization()
{
    delete viewer;

    delete ui;
}

void AbstractTableVisualization::selectedEventChanged(QString event)
{
    currentEvent = event;
    updatePage();
}

void AbstractTableVisualization::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    if(isActive)
        updatePage();

    /*if(isActive)
    {
        decrement = 0;

        int cursorMoving = currentLine - topLinePos;

        updatePage(cursorMoving);
    }*/
}

bool AbstractTableVisualization::eventFilter(QObject *target, QEvent *event)
{
}

void AbstractTableVisualization::scrollBarMoving(int value)
{
    topLinePos = value;

    /*int cursorMoving = 0;

    if(wheel)
    {
        cursorMoving = currentLine - topLinePos;
        updatePage(cursorMoving);
        wheel = false;
    }
    else
    {

        updatePage();
    }*/

    updatePage();
}

int AbstractTableVisualization::linesOnPage()
{
    viewer->setRowCount(1);
    viewer->setItem(0, 0, new QTableWidgetItem("test"));
    viewer->resizeRowToContents(0);

    return (viewer->viewport()->size().height() - TVIEWER_PADDING_KOEFF) / viewer->visualItemRect(viewer->item(0,0)).size().height();
}
