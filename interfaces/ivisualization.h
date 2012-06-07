/*
 * ivisualization.h
 *
 *  Created on: May 8, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#ifndef IVISUALIZATION_H_
#define IVISUALIZATION_H_

#include <QtGui/QWidget>
#include <QString>

#include "ilog.h"
#include "iproject.h"
#include "ivisualizationsettings.h"

class IVisualization
{
public:
    virtual IVisualizationSettings *visualizationSettings(QString name) = 0;

    virtual void activity(bool status) = 0;
    virtual void update(IProject *project, ILog *log) = 0;
    virtual QWidget *getWidget() = 0;
    virtual void fromLine(qint64 line) = 0;
    virtual ~IVisualization() { }
};

#endif /* IVISUALIZATION_H_ */
