/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef TEXTAPPEARANCECOLORSANDFONTSSETTINGS_H
#define TEXTAPPEARANCECOLORSANDFONTSSETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QAbstractButton>
#include <QFont>
#include <QFontDialog>
#include <QDebug>

#include "ui_abstracttextappearancecolorsandfontssettings.h"
#include "abstracttextappearancecolorsandfontssettings.h"
#include "ivisualizationsettings.h"

class TextAppearanceColorsAndFontsSettings : public AbstractTextAppearanceColorsAndFontsSettings, public IVisualizationSettings
{
    Q_OBJECT

public:
    explicit TextAppearanceColorsAndFontsSettings(QWidget *parent = 0);

    // interface methods

    /*virtual*/ void setSettingsName(QString name);
    /*virtual*/ void showCurrentSettings();
    /*virtual*/ void applySettings();
    /*virtual*/ QWidget *getWidget();

    virtual ~TextAppearanceColorsAndFontsSettings();

private:
    QSettings settings;

    void updatePreviewText();

    void showDefaultSettings();

private slots:
    void buttonClicked(QAbstractButton *button);
    void buttonChangeFontClicked();
};

#endif // TEXTAPPEARANCECOLORSANDFONTSSETTINGS_H
