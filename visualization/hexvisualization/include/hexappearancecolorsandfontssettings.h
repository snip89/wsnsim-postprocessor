/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef HEXAPPEARANCECOLORSANDFONTSSETTINGS_H
#define HEXAPPEARANCECOLORSANDFONTSSETTINGS_H

#include <QWidget>

#include "ui_hexappearancecolorsandfontssettings.h"

namespace Ui {
    class HexAppearanceColorsAndFontsSettings;
}

class HexAppearanceColorsAndFontsSettings : public QWidget
{
    Q_OBJECT

public:
    explicit HexAppearanceColorsAndFontsSettings(QWidget *parent = 0);
    virtual ~HexAppearanceColorsAndFontsSettings();

private:
    Ui::HexAppearanceColorsAndFontsSettings *ui;
};

#endif // HEXAPPEARANCECOLORSANDFONTSSETTINGS_H
