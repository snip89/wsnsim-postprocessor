#ifndef REALTIMETEXTVISUALIZATION_H
#define REALTIMETEXTVISUALIZATION_H

#include "irealtimevisualization.h"
#include "abstractrealtimetextvisualization.h"
#include "records.h"
#include "staticrecordsreader.h"

class RealTimeTextVisualization : public IRealTimeVisualization, protected AbstractRealTimeTextVisualization
{
public:
    explicit RealTimeTextVisualization(QWidget *parent = 0);

    // interface methods

    /*virtual*/ void update(IProject *project, QUdpSocket *socket);
    /*virtual*/ QWidget *getWidget();

    /*virtual*/ void addRecord(QByteArray byteRecord);

    virtual ~RealTimeTextVisualization();
};

#endif // REALTIMETEXTVISUALIZATION_H
