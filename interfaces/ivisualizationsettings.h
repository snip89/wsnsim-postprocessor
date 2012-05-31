/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef IVISUALIZATIONSETTINGS_H
#define IVISUALIZATIONSETTINGS_H

#include <QWidget>
#include <QObject>

class IVisualizationSettings
{
public:
    virtual void showCurrentSettings() = 0;
    virtual void applySettings() = 0;
    virtual QWidget *getWidget() = 0;
    virtual ~IVisualizationSettings() { }
};

#endif // IVISUALIZATIONSETTINGS_H
