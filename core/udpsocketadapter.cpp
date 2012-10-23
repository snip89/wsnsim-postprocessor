#include "udpsocketadapter.h"

UdpSocketAdapter::UdpSocketAdapter(QUdpSocket *socket)
{
    udpSocket = socket;

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

UdpSocketAdapter::~UdpSocketAdapter()
{
    delete udpSocket;
}

void UdpSocketAdapter::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        emit dataRecieved(datagram);
    }
}
