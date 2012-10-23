#ifndef ABSTRACTREALTIMETEXTVISUALIZATION_H
#define ABSTRACTREALTIMETEXTVISUALIZATION_H

#include <QUdpSocket>

#include "iproject.h"
#include "viewer.h"
#include "udpsocketadapter.h"
#include "ui_abstractrealtimetextvisualization.h"

namespace Ui {
    class AbstractRealTimeTextVisualization;
}

class AbstractRealTimeTextVisualization : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractRealTimeTextVisualization(QWidget *parent = 0);
    virtual ~AbstractRealTimeTextVisualization();
protected:
    Viewer *viewer;

    IProject *currentProject;
    UdpSocketAdapter *currentSocketAdapter;

    quint64 recordsCount;
    quint64 recordsLimit;

    QSettings settings;

    Ui::AbstractRealTimeTextVisualization *ui;
private slots:
    virtual void addRecord(QByteArray byteRecord) = 0;
};

#endif // ABSTRACTREALTIMETEXTVISUALIZATION_H
