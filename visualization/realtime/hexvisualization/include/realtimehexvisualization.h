#ifndef REALTIMEHEXVISUALIZATION_H
#define REALTIMEHEXVISUALIZATION_H

#include "irealtimevisualization.h"
#include "abstractrealtimetextvisualization.h"
#include "records.h"
#include "staticrecordsreader.h"

class RealTimeHexVisualization : public IRealTimeVisualization, protected AbstractRealTimeTextVisualization
{
public:
    explicit RealTimeHexVisualization(QWidget *parent = 0);

    // interface methods

    /*virtual*/ void activity(bool status);
    /*virtual*/ void update(IProject *project, QUdpSocket *socket);
    /*virtual*/ QWidget *getWidget();

    /*virtual*/ void addRecord(QByteArray byteRecord);

    virtual ~RealTimeHexVisualization();
};

#endif // REALTIMEHEXVISUALIZATION_H
