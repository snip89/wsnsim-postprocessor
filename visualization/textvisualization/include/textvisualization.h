/*
 * textvisualization.h
 *
 *  Created on: May 8, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#ifndef TEXTVISUALIZATION_H_
#define TEXTVISUALIZATION_H_

#include <QWidget>
#include <QSize>
#include <QFont>
#include <QResizeEvent>
#include <QTextEdit>
#include <QVariant>
#include <QScrollBar>
#include <QByteArray>
#include <QObject>
#include <QEvent>
#include <QDebug>
#include <QCoreApplication>

#include "ivisualization.h"
#include "iproject.h"
#include "ilog.h"
#include "staticrecordsreader.h"

namespace Ui {
    class TextVisualization;
}

class TextVisualization : public QWidget, public IVisualization
{
    Q_OBJECT
public:
    explicit TextVisualization(QWidget *parent = 0);

    // interface methods
    /*virtual*/ void activity(bool status);
    /*virtual*/ void update(IProject *project, ILog *log);
    /*virtual*/ QWidget *getWidget();

    /*virtual*/ void resizeEvent(QResizeEvent *e);
    /*virtual*/ bool eventFilter(QObject *target, QEvent *event);

    virtual ~TextVisualization();
private:
    ILog *currentLog;
    IProject *currentProject;

    bool isActive;
    qint64 topLinePos;

    void initObjectsConnections();

    void updatePage();
    int linesOnPage();

    Ui::TextVisualization *ui;
private slots:
    void scrollBarMoving(int value);
};

#endif /* TEXTVISUALIZATION_H_ */
