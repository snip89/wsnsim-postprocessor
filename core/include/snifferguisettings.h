/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef SNIFFERGUISETTINGS_H
#define SNIFFERGUISETTINGS_H

#include <QWidget>
#include <QString>
#include <QSettings>
#include <QAbstractButton>
#include <QMessageBox>
#include <QSessionManager>
#include <QDebug>

#include "ui_snifferguisettings.h"

namespace Ui {
    class SnifferGuiSettings;
}

class SnifferGuiSettings : public QWidget
{
    Q_OBJECT

public:
    SnifferGuiSettings(QWidget *parent = 0);

    void setSettingsName(QString name);
    void showCurrentSettings();
    void applySettings();

    ~SnifferGuiSettings();
private:
    Ui::SnifferGuiSettings *ui;

    QSettings settings;

    void showDefaultSettings();
private slots:
    void buttonClicked(QAbstractButton *button);
};

#endif // SNIFFERGUISETTINGS_H