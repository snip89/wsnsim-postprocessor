#ifndef ABSTRACTREALTIMETABLEVISUALIZATION_H
#define ABSTRACTREALTIMETABLEVISUALIZATION_H

#include <QUdpSocket>
#include <QSettings>

#include "iproject.h"
#include "tableviewer.h"
#include "udpsocketadapter.h"
#include "ui_abstractrealtimetablevisualization.h"

namespace Ui {
    class AbstractRealTimeTableVisualization;
}

class AbstractRealTimeTableVisualization : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractRealTimeTableVisualization(QWidget *parent = 0);
    virtual ~AbstractRealTimeTableVisualization();
protected:
    TableViewer *viewer;

    IProject *currentProject;
    UdpSocketAdapter *currentSocketAdapter;

    quint64 recordsCount;
    quint64 recordsLimit;

    QSettings settings;

    Ui::AbstractRealTimeTableVisualization *ui;
private slots:
    virtual void addRecord(QByteArray byteRecord) = 0;
};

#endif // ABSTRACTREALTIMETABLEVISUALIZATION_H
