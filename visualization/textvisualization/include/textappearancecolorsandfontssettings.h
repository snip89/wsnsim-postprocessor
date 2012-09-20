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
#include <QListWidgetItem> 
#include <QColorDialog>
#include <QDebug>

#include "ui_abstracttextappearancecolorsandfontssettings.h"
#include "abstracttextappearancecolorsandfontssettings.h"
// #include "ivisualizationsettings.h"

class TextAppearanceColorsAndFontsSettings : public AbstractTextAppearanceColorsAndFontsSettings
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

    QListWidgetItem *itemMainText;
    QListWidgetItem *itemCurrentLine;

    void updatePreviewText();

    void createListWidgetItems();
    void deleteListWidgetItems();

    void showDefaultSettings();

private slots:
    void buttonClicked(QAbstractButton *button);
    void buttonChangeFontClicked();
    void buttonChangeForegroundClicked();
    void buttonChangeBackgroundClicked();
};

#endif // TEXTAPPEARANCECOLORSANDFONTSSETTINGS_H
