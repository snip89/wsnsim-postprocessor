#ifndef ABSTRACTREALTIMETEXTVISUALIZATION_H
#define ABSTRACTREALTIMETEXTVISUALIZATION_H

#include <QUdpSocket>

#include "iproject.h"
#include "viewer.h"
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
    QUdpSocket *currentSocket;

    quint64 recordsCount;
    quint64 recordsLimit;

    bool firstTime;

    QSettings settings;

    virtual void addRecord(QByteArray byteRecord) = 0;

    Ui::AbstractRealTimeTextVisualization *ui;
private slots:
    void readPendingDatagrams();
};

#endif // ABSTRACTREALTIMETEXTVISUALIZATION_H
