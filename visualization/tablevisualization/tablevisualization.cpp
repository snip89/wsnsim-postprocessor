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
}

void TableVisualization::update()
{
}

QWidget *TableVisualization::getWidget()
{
    return this;
}

void TableVisualization::fromLine(qint64 line)
{
}

TableVisualization::~TableVisualization()
{
}

void TableVisualization::setSettings(QSettings &someSettings)
{
}
