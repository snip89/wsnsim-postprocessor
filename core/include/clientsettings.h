#ifndef CLIENTSETTINGS_H
#define CLIENTSETTINGS_H

#include <QWidget>

#include "ui_clientsettings.h"
#include "staticcoreutils.h"
#include "irealtimesettings.h"

namespace Ui {
    class ClientSettings;
}

class ClientSettings : public QWidget
{
    Q_OBJECT
public:
    ClientSettings(QWidget *parent = 0);

    void setSettingsName(QString name);
    void showCurrentSettings();
    void applySettings();
    QString client();

    ~ClientSettings();
private:
    Ui::ClientSettings *ui;

    void showDefaultSettings();

private slots:
    void buttonClicked(QAbstractButton *button);
};

#endif // CLIENTSETTINGS_H
