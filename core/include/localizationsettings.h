/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef LOCALIZATIONSETTINGS_H
#define LOCALIZATIONSETTINGS_H

#include <QWidget>
#include <QString>
#include <QSettings>
#include <QAbstractButton>
#include <QMessageBox>
#include <QSessionManager>

#include "ui_localizationsettings.h"

namespace Ui {
    class LocalizationSettings;
}

class LocalizationSettings : public QWidget
{
    Q_OBJECT

public:
    LocalizationSettings(QWidget *parent = 0);

    void setSettingsName(QString name);
    void showCurrentSettings();
    void applySettings();

    ~LocalizationSettings();
private:
    Ui::LocalizationSettings *ui;

    QSettings settings;

    void showDefaultSettings();
private slots:
    void buttonClicked(QAbstractButton *button);
};

#endif // LOCALIZATIONSETTINGS_H
