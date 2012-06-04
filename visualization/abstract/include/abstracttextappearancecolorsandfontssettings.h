/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef ABSTRACTTEXTAPPEARANCECOLORSANDFONTSSETTINGS_H
#define ABSTRACTTEXTAPPEARANCECOLORSANDFONTSSETTINGS_H

#include <QWidget>
#include <QObject>

#include "ui_abstracttextappearancecolorsandfontssettings.h"

namespace Ui {
    class AbstractTextAppearanceColorsAndFontsSettings;
}

class AbstractTextAppearanceColorsAndFontsSettings : public QWidget
{
    Q_OBJECT

public:
    explicit AbstractTextAppearanceColorsAndFontsSettings(QWidget *parent = 0);

    virtual ~AbstractTextAppearanceColorsAndFontsSettings();

protected:
    Ui::AbstractTextAppearanceColorsAndFontsSettings *ui;
};

#endif // ABSTRACTTEXTAPPEARANCECOLORSANDFONTSSETTINGS_H
