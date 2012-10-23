#include "abstractrealtimetextvisualization.h"

AbstractRealTimeTextVisualization::AbstractRealTimeTextVisualization(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbstractRealTimeTextVisualization)
{
    ui->setupUi(this);

    viewer = new Viewer();

    ui->vAreaGridLayout->addWidget(viewer);

    viewer->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    viewer->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    recordsCount = 0;
}

AbstractRealTimeTextVisualization::~AbstractRealTimeTextVisualization()
{
    delete viewer;
    delete ui;
}
