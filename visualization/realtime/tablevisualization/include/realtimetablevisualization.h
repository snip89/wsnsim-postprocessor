#ifndef REALTIMETABLEVISUALIZATION_H
#define REALTIMETABLEVISUALIZATION_H

#include "irealtimevisualization.h"
#include "abstractrealtimetablevisualization.h"
#include "records.h"
#include "staticrecordsreader.h"
#include "udpsocketadapter.h"

class RealTimeTableVisualization : public IRealTimeVisualization, protected AbstractRealTimeTableVisualization
{
public:
    explicit RealTimeTableVisualization(QWidget *parent = 0);

    // interface methods

    /*virtual*/ void stop();
    /*virtual*/ void update(IProject *project, UdpSocketAdapter *socketAdapter, Format *format);
    /*virtual*/ void update();
    /*virtual*/ QWidget *getWidget();

    /*virtual*/ void addRecord(QByteArray byteRecord);

    virtual ~RealTimeTableVisualization();
private:
    QStringList eventTypes;
};

#endif // REALTIMETABLEVISUALIZATION_H
