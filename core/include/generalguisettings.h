/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef GENERALGUISETTINGS_H
#define GENERALGUISETTINGS_H

#include <QWidget>
#include <QString>
#include <QSettings>
#include <QAbstractButton>

#include "ui_generalguisettings.h"

namespace Ui {
    class GeneralGuiSettings;
}

class GeneralGuiSettings : public QWidget
{
    Q_OBJECT

public:
    GeneralGuiSettings(QWidget *parent = 0);

    void setSettingsName(QString name);
    void showCurrentSettings();
    void applySettings();

    ~GeneralGuiSettings();

private:
    Ui::GeneralGuiSettings *ui;

    QSettings settings;

    void showDefaultSettings();

private slots:
    void buttonClicked(QAbstractButton *button);
};

#endif // GENERALGUISETTINGS_H
