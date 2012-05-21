/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef ABSTRACTTEXTVISUALIZATION_H
#define ABSTRACTTEXTVISUALIZATION_H

#include <QWidget>
#include <QObject>
#include <QEvent>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QDebug>

#include "ilog.h"
#include "iproject.h"

namespace Ui {
    class AbstractTextVisualization;
}

class AbstractTextVisualization : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractTextVisualization(QWidget *parent = 0);

    /*virtual*/ void resizeEvent(QResizeEvent *e);
    /*virtual*/ bool eventFilter(QObject *target, QEvent *event);

    virtual ~AbstractTextVisualization();

protected:
    ILog *currentLog;
    IProject *currentProject;

    bool isActive;
    qint64 topLinePos;

    virtual void updatePage() = 0;

    int linesOnPage();

    Ui::AbstractTextVisualization *ui;

private slots:
    void scrollBarMoving(int value);
};

#endif // ABSTRACTTEXTVISUALIZATION_H
