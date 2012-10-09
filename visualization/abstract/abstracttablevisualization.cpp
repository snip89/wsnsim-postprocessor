#include "abstracttablevisualization.h"

AbstractTableVisualization::AbstractTableVisualization(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::AbstractTableVisualization)
{
    ui->setupUi(this);

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
