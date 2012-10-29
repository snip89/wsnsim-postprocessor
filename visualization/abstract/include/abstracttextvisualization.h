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
#include "formatParams.h"

#include "viewer.h"

namespace Ui {
    class AbstractTextVisualization;
}

class AbstractTextVisualization : public QWidget
{
    Q_OBJECT
public:
    enum Direction
    {
        Up,
        Down
    };
    explicit AbstractTextVisualization(QString group, QWidget *parent = 0);

    /*virtual*/ void resizeEvent(QResizeEvent *e);
    /*virtual*/ bool eventFilter(QObject *target, QEvent *event);

    virtual ~AbstractTextVisualization();

protected:
    Direction direction;

    ILog *currentLog;
    IProject *currentProject;
    QList<Format*> formats;

    qint64 currentLine;

    bool isActive;
    qint64 topLinePos;
    bool wheel;

    int decrement;

    virtual void updatePage() = 0;
    virtual void updatePage(int cursorMoving) = 0;

    int linesOnPage(int decrement);

    Viewer *viewer;

    QSettings settings;

    QString name;

    Ui::AbstractTextVisualization *ui;

private slots:
    void scrollBarMoving(int value);
    void cursorPosChanging();
//    void hscrollBarRangeChanging(int min, int max);
};

#endif // ABSTRACTTEXTVISUALIZATION_H
