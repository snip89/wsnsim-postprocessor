/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef HEXAPPEARANCECOLORSANDFONTSSETTINGS_H
#define HEXAPPEARANCECOLORSANDFONTSSETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QAbstractButton>
#include <QFont>
#include <QFontDialog>
#include <QListWidgetItem>
#include <QDebug>

#include "ui_abstracttextappearancecolorsandfontssettings.h"
#include "abstracttextappearancecolorsandfontssettings.h"
// #include "ivisualizationsettings.h"

class HexAppearanceColorsAndFontsSettings : public AbstractTextAppearanceColorsAndFontsSettings
{
    Q_OBJECT

public:
    explicit HexAppearanceColorsAndFontsSettings(QWidget *parent = 0);

    // interface methods

    /*virtual*/ void setSettingsName(QString name);
    /*virtual*/ void showCurrentSettings();
    /*virtual*/ void applySettings();
    /*virtual*/ QWidget *getWidget();

    virtual ~HexAppearanceColorsAndFontsSettings();

private:
    QSettings settings;

    QListWidgetItem *itemMainText;
    QListWidgetItem *itemCurrentLine;

    void createListWidgetItems();
    void deleteListWidgetItems();

    // void updatePreviewText();
    void updateColorsList();

    void showDefaultSettings();

private slots:
    void buttonClicked(QAbstractButton *button);
    void buttonChangeFontClicked();
};

#endif // HEXAPPEARANCECOLORSANDFONTSSETTINGS_H
