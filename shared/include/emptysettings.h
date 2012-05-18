#ifndef EMPTYSETTINGS_H
#define EMPTYSETTINGS_H

#include <QtGui/QWidget>
#include <QString>
#include "ui_emptysettings.h"

class EmptySettings : public QWidget
{
    Q_OBJECT

public:
    EmptySettings(QWidget *parent = 0);

    void setSettingsName(QString name);

    ~EmptySettings();

private:
    Ui::EmptySettingsClass ui;
};

#endif // EMPTYSETTINGS_H
