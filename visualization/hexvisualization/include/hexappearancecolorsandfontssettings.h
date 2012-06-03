/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef HEXAPPEARANCECOLORSANDFONTSSETTINGS_H
#define HEXAPPEARANCECOLORSANDFONTSSETTINGS_H

#include <QWidget>

#include "ui_hexappearancecolorsandfontssettings.h"
#include "ivisualizationsettings.h"

namespace Ui {
    class HexAppearanceColorsAndFontsSettings;
}

class HexAppearanceColorsAndFontsSettings : public QWidget, public IVisualizationSettings
{
    Q_OBJECT

public:
    explicit HexAppearanceColorsAndFontsSettings(QWidget *parent = 0);

    /*virtual*/ void setSettingsName(QString name);
    /*virtual*/ void showCurrentSettings();
    /*virtual*/ void applySettings();
    /*virtual*/ QWidget *getWidget();

    virtual ~HexAppearanceColorsAndFontsSettings();

private:
    Ui::HexAppearanceColorsAndFontsSettings *ui;
};

#endif // HEXAPPEARANCECOLORSANDFONTSSETTINGS_H
