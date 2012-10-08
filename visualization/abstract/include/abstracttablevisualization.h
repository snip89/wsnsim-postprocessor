#ifndef ABSTRACTTABLEVISUALIZATION_H
#define ABSTRACTTABLEVISUALIZATION_H

#include <QWidget>

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
    Ui::AbstractTableVisualization *ui;
};

#endif // ABSTRACTTABLEVISUALIZATION_H
