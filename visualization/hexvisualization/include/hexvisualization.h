/*
 * hexvisualization.h
 *
 *  Created on: May 8, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#ifndef HEXVISUALIZATION_H_
#define HEXVISUALIZATION_H_

#include <QWidget>
#include <QSize>
#include <QFont>
#include <QResizeEvent>
#include <QTextEdit>
#include <QVariant>
#include <QScrollBar>
#include <QByteArray>
#include <QAbstractSlider>
#include <QDebug>

#include "ivisualization.h"
#include "iproject.h"
#include "ilog.h"
#include "staticrecordsreader.h"

namespace Ui {
class HexVisualization;
}

class HexVisualization : public QWidget, public IVisualization
{
    Q_OBJECT
public:
    explicit HexVisualization(QWidget *parent = 0);

    // interface methods
    /*virtual*/ void activity(bool status);
    /*virtual*/ void update(IProject *project, ILog *log);
    /*virtual*/ QWidget *getWidget();

    /*virtual*/ void resizeEvent(QResizeEvent *e);
    /*virtual*/ bool eventFilter(QObject *target, QEvent *event);

    virtual ~HexVisualization();
private:
    ILog *currentLog;
    IProject *currentProject;

    bool isActive;
    qint64 topLinePos;

    void initObjectsConnections();

    void updatePage();
    int linesOnPage();

    Ui::HexVisualization *ui;
private slots:
    void scrollBarMoving(int value);
};

#endif /* HEXVISUALIZATION_H_ */
