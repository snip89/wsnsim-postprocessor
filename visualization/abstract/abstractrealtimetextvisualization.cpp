#include "abstractrealtimetextvisualization.h"

AbstractRealTimeTextVisualization::AbstractRealTimeTextVisualization(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbstractRealTimeTextVisualization)
{
    ui->setupUi(this);

    viewer = new Viewer();

    ui->vAreaGridLayout->addWidget(viewer);

    connect(currentSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));

    recordsCount = 0;
}

AbstractRealTimeTextVisualization::~AbstractRealTimeTextVisualization()
{
    delete viewer;
    delete ui;
}

void AbstractRealTimeTextVisualization::readPendingDatagrams()
{
    while (currentSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(currentSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        currentSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        // update record here
    }
}
