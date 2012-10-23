#include "abstractrealtimetablevisualization.h"

AbstractRealTimeTableVisualization::AbstractRealTimeTableVisualization(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbstractRealTimeTableVisualization)
{
    ui->setupUi(this);

    viewer = new TableViewer();

    ui->vAreaGridLayout->addWidget(viewer);

    viewer->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    viewer->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    recordsCount = 0;
}

AbstractRealTimeTableVisualization::~AbstractRealTimeTableVisualization()
{
    delete viewer;
    delete ui;
}
