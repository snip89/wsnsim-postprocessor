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
#include <QLayout>
#include <QGridLayout>
#include <QSettings>
#include <QString>
#include <QDebug>
#include <QFontMetricsF>
#include <QSizeF>

#include "ilog.h"
#include "iproject.h"

#include "viewer.h"

enum Direction
{
    Up,
    Down
};

namespace Ui {
    class AbstractTextVisualization;
}

class AbstractTextVisualization : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractTextVisualization(QString group, QWidget *parent = 0);

    /*virtual*/ void resizeEvent(QResizeEvent *e);
    /*virtual*/ bool eventFilter(QObject *target, QEvent *event);

    virtual ~AbstractTextVisualization();

protected:
    Direction direction;

    ILog *currentLog;
    IProject *currentProject;

    bool isActive;
    qint64 topLinePos;

    virtual void updatePage() = 0;

    int linesOnPage();

    Viewer *viewer;

    QSettings settings;

    Ui::AbstractTextVisualization *ui;

private slots:
    void scrollBarMoving(int value);
//    void hscrollBarRangeChanging(int min, int max);
};

#endif // ABSTRACTTEXTVISUALIZATION_H
