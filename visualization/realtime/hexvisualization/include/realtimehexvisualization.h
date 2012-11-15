#ifndef REALTIMEHEXVISUALIZATION_H
#define REALTIMEHEXVISUALIZATION_H

#include "irealtimevisualization.h"
#include "abstractrealtimetextvisualization.h"
#include "records.h"
#include "staticrecordsreader.h"
#include "udpsocketadapter.h"

class RealTimeHexVisualization : public IRealTimeVisualization, protected AbstractRealTimeTextVisualization
{
public:
    explicit RealTimeHexVisualization(QWidget *parent = 0);

    // interface methods

    /*virtual*/ void stop();
    /*virtual*/ void update(IProject *project, UdpSocketAdapter *socketAdapter,  QList<Format*> formats);
    /*virtual*/ void update();
    /*virtual*/ void searchNext(QString str);
    /*virtual*/ void searchPrevious(QString str);
    /*virtual*/ QWidget *getWidget();

    /*virtual*/ void addRecord(QByteArray byteRecord);

    virtual ~RealTimeHexVisualization();
};

#endif // REALTIMEHEXVISUALIZATION_H
