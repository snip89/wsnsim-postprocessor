#ifndef IREALTIMEVISUALIZATION_H
#define IREALTIMEVISUALIZATION_H

#include <QtGui/QWidget>
#include <QtNetwork/QUdpSocket>
#include <QString>

#include "iproject.h"
#include "udpsocketadapter.h"

class IRealTimeVisualization
{
public:
    virtual void stop() = 0;
    virtual void update(IProject *project, UdpSocketAdapter *socketAdapter) = 0;
    virtual void update() = 0;
    virtual QWidget *getWidget() = 0;
    virtual ~IRealTimeVisualization() { }
};

#endif // IREALTIMEVISUALIZATION_H
