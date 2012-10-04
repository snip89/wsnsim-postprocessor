/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef SNIFFERCORESETTINGS_H
#define SNIFFERCORESETTINGS_H

#include <QWidget>
#include <QString>
#include <QSettings>
#include <QAbstractButton>
#include <QMessageBox>
#include <QSessionManager>
#include <QDebug>

#include "ui_sniffercoresettings.h" 

namespace Ui {
    class SnifferCoreSettings;
}

class SnifferCoreSettings : public QWidget
{
    Q_OBJECT

public:
    SnifferCoreSettings(QWidget *parent = 0);

    void setSettingsName(QString name);
    void showCurrentSettings();
    void applySettings();

    ~SnifferCoreSettings();
private:
    Ui::SnifferCoreSettings *ui;

    QSettings settings;

    void showDefaultSettings();
private slots:
    void buttonClicked(QAbstractButton *button);
};

#endif // SNIFFERCORESETTINGS_H