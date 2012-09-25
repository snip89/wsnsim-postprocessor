/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef TEXTGUISETTINGS_H
#define TEXTGUISETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QAbstractButton>
#include <QDebug>

#include "ui_abstracttextguisettings.h"
#include "abstracttextguisettings.h"

class TextGuiSettings : public AbstractTextGuiSettings
{
	Q_OBJECT
public:
	explicit TextGuiSettings(QWidget *parent = 0);

	// interface methods

    /*virtual*/ void setSettingsName(QString name);
    /*virtual*/ void showCurrentSettings();
    /*virtual*/ void applySettings();

    virtual ~TextGuiSettings();

private:
    QSettings settings;

    void showDefaultSettings();

private slots:
    void buttonClicked(QAbstractButton *button);
};

#endif // TEXTGUISETTINGS_H