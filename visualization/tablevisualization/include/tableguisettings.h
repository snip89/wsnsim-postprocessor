#ifndef TABLEGUISETTINGS_H
#define TABLEGUISETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QAbstractButton>
#include <QDebug>
#include <QtCore>

#include "ui_tableguisettings.h"

namespace Ui {
    class TableGuiSettings;
}

class TableGuiSettings : public QWidget
{
    Q_OBJECT
public:
    explicit TableGuiSettings(QWidget *parent = 0);

    void setSettingsName(QString name);
    void showCurrentSettings();
    void applySettings();

    virtual ~TableGuiSettings();

private:
    QSettings settings;

    Ui::TableGuiSettings *ui;

    void showDefaultSettings();

private slots:
    void buttonClicked(QAbstractButton *button);
};

#endif // TABLEGUISETTINGS_H
