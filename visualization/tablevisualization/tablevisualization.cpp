#include "tablevisualization.h"

TableVisualization::TableVisualization(QWidget *parent)
    : AbstractTableVisualization(parent)
{
}

void TableVisualization::activity(bool status)
{
}

void TableVisualization::update(IProject *project, ILog *log)
{
    updatePage();
}

void TableVisualization::update()
{
    updatePage();
}

QWidget *TableVisualization::getWidget()
{
    return this;
}

void TableVisualization::fromLine(qint64 line)
{
}

void TableVisualization::updatePage()
{
    viewer->setColumnCount(10);
    viewer->setRowCount(10);

    ui->horizontalScrollBar->setMinimum(viewer->horizontalScrollBar()->minimum());
    ui->horizontalScrollBar->setMaximum(viewer->horizontalScrollBar()->maximum());
}

void TableVisualization::updatePage(int cursorMoving)
{
    updatePage();
}

TableVisualization::~TableVisualization()
{
}

void TableVisualization::setSettings(QSettings &someSettings)
{
}
