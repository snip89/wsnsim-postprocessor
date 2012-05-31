/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef IVISUALIZATIONSETTINGS_H
#define IVISUALIZATIONSETTINGS_H

class IVisualizationSettings
{
public:
    virtual void showCurrentSettings() = 0;
    virtual void applySettings() = 0;
    virtual ~IVisualizationSettings() { }
};

#endif // IVISUALIZATIONSETTINGS_H
