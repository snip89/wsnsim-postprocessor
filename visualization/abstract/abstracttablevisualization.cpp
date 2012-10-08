#include "abstracttablevisualization.h"

AbstractTableVisualization::AbstractTableVisualization(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::AbstractTableVisualization)
{
    ui->setupUi(this);
}

AbstractTableVisualization::~AbstractTableVisualization()
{
    delete ui;
}
