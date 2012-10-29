#ifndef ABSTRACTTABLEVISUALIZATION_H
#define ABSTRACTTABLEVISUALIZATION_H

#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QSettings>
#include <QWheelEvent>

#include "tableviewer.h"
#include "ilog.h"
#include "iproject.h"
#include "formatParams.h"

#include "ui_abstracttablevisualization.h"

namespace Ui {
    class AbstractTableVisualization;
}

class AbstractTableVisualization : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractTableVisualization(QWidget *parent = 0);

    /*virtual*/ void resizeEvent(QResizeEvent *e);
    /*virtual*/ bool eventFilter(QObject *target, QEvent *event);

    virtual ~AbstractTableVisualization();
protected:
    TableViewer *viewer;

    ILog *currentLog;
    IProject *currentProject;
    ILog *currentEventLog;
    QList<Format*> formats;

    QString currentEvent;

    bool isActive;
    qint64 topLinePos;

    int currentRow;
    int currentColumn;

    QSettings settings;

    virtual void updatePage() = 0;
    virtual void updatePage(int cursorMoving) = 0;

    Ui::AbstractTableVisualization *ui;

    int linesOnPage();
private slots:
    void scrollBarMoving(int value);
};

#endif // ABSTRACTTABLEVISUALIZATION_H
