/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef HEXGUISETTINGS_H
#define HEXGUISETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QAbstractButton>
#include <QDebug>

#include "ui_abstracttextguisettings.h"
#include "abstracttextguisettings.h"

class HexGuiSettings : public AbstractTextGuiSettings
{
	Q_OBJECT
public:
	explicit HexGuiSettings(QWidget *parent = 0);

	// interface methods

    /*virtual*/ void setSettingsName(QString name);
    /*virtual*/ void showCurrentSettings();
    /*virtual*/ void applySettings();

    virtual ~HexGuiSettings();

private:
    QSettings settings;

    void showDefaultSettings();

private slots:
    void buttonClicked(QAbstractButton *button);
};

#endif // HEXGUISETTINGS_H