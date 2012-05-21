/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef GENERALCORESETTINGS_H
#define GENERALCORESETTINGS_H

#include <QWidget>
#include <QString>
#include <QSettings>
#include <QAbstractButton>
#include <QMessageBox>

#include "ui_generalcoresettings.h"
#include "ostools.h"

namespace Ui {
    class GeneralCoreSettings;
}

class GeneralCoreSettings : public QWidget
{
    Q_OBJECT

public:
    GeneralCoreSettings(QWidget *parent = 0);

    void setSettingsName(QString name);
    void showCurrentSettings();
    void applySettings();

    ~GeneralCoreSettings();

private:
    Ui::GeneralCoreSettings *ui;

    QSettings settings;

    void showDefaultSettings();

private slots:
    void buttonClicked(QAbstractButton *button);
};

#endif // GENERALCORESETTINGS_H
