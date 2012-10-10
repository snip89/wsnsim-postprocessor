#include "abstracttablevisualization.h"

AbstractTableVisualization::AbstractTableVisualization(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::AbstractTableVisualization)
{
    ui->setupUi(this);

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

int AbstractTableVisualization::linesOnPage(int decrement)
{
    viewer->setRowCount(1);
    viewer->setItem(0, 0, new QTableWidgetItem("test"));
    viewer->resizeRowToContents(0);

    return (viewer->viewport()->size().height() - VIEWER_PADDING_KOEFF) / viewer->visualItemRect(viewer->item(0,0)).size().height();
}
