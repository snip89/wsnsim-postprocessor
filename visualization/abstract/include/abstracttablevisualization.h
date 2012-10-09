#ifndef ABSTRACTTABLEVISUALIZATION_H
#define ABSTRACTTABLEVISUALIZATION_H

#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

#include "tableviewer.h"
#include "ui_abstracttablevisualization.h"

namespace Ui {
    class AbstractTableVisualization;
}

class AbstractTableVisualization : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractTableVisualization(QWidget *parent = 0);
    virtual ~AbstractTableVisualization();
protected:
    TableViewer *viewer;

    //ILog *currentLog;
    //IProject *currentProject;

    bool isActive;
    qint64 topLinePos;

    virtual void updatePage() = 0;
    virtual void updatePage(int cursorMoving) = 0;

    Ui::AbstractTableVisualization *ui;
};

#endif // ABSTRACTTABLEVISUALIZATION_H
