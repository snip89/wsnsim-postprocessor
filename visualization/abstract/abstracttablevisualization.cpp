#include "abstracttablevisualization.h"

AbstractTableVisualization::AbstractTableVisualization(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::AbstractTableVisualization)
{
}

AbstractTableVisualization::~AbstractTableVisualization()
{
    delete ui;
}
