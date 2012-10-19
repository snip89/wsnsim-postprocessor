#ifndef IREALTIMEVISUALIZATION_H
#define IREALTIMEVISUALIZATION_H

#include <QtGui/QWidget>
#include <QtNetwork/QUdpSocket>
#include <QString>

#include "iproject.h"

class IRealTimeVisualization
{
public:
    virtual void update(IProject *project, QUdpSocket *socket) = 0;
    virtual QWidget *getWidget() = 0;
    virtual ~IRealTimeVisualization() { }
};

#endif // IREALTIMEVISUALIZATION_H
