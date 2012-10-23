#ifndef UDPSOCKETADAPTER_H
#define UDPSOCKETADAPTER_H

#include <QtNetwork/QUdpSocket>
#include <QObject>
#include <QByteArray>

class UdpSocketAdapter : public QObject
{
    Q_OBJECT
public:
    UdpSocketAdapter(QUdpSocket *socket);

    virtual ~UdpSocketAdapter();

signals:
    void dataRecieved(QByteArray data);

private slots:
    void readPendingDatagrams();

private:
    QUdpSocket *udpSocket;
};

#endif // UDPSOCKETADAPTER_H
