#include "clientsettings.h"

ClientSettings::ClientSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientSettings)
{
    ui->setupUi(this);
}

void ClientSettings::setSettingsName(QString name)
{
    ui->settingsNameLabel->setText(name);
}

void ClientSettings::showCurrentSettings()
{
}

void ClientSettings::applySettings()
{
}

QString ClientSettings::client()
{
    return ui->settingsNameLabel->text();
}

ClientSettings::~ClientSettings()
{
    delete ui;
}

void ClientSettings::showDefaultSettings()
{
}

void ClientSettings::buttonClicked(QAbstractButton *button)
{
}
