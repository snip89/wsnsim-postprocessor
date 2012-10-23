#ifndef REALTIMETEXTVISUALIZATION_H
#define REALTIMETEXTVISUALIZATION_H

#include "irealtimevisualization.h"
#include "abstractrealtimetextvisualization.h"
#include "records.h"
#include "staticrecordsreader.h"
#include "udpsocketadapter.h"

class RealTimeTextVisualization : public IRealTimeVisualization, protected AbstractRealTimeTextVisualization
{
public:
    explicit RealTimeTextVisualization(QWidget *parent = 0);

    // interface methods

    /*virtual*/ void stop();
    /*virtual*/ void update(IProject *project, UdpSocketAdapter *socketAdapter);
    /*virtual*/ void update();
    /*virtual*/ QWidget *getWidget();

    /*virtual*/ void addRecord(QByteArray byteRecord);

    virtual ~RealTimeTextVisualization();
};

#endif // REALTIMETEXTVISUALIZATION_H
