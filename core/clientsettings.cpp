#include "clientsettings.h"

ClientSettings::ClientSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientSettings)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

void ClientSettings::setSettingsName(QString name)
{
    ui->settingsNameLabel->setText(name);
}

void ClientSettings::showCurrentSettings()
{
    IRealTimeSettings *settings = StaticCoreUtils::getRealTimeSettings();

    ui->ipLineEdit->setText(settings->ip(ui->settingsNameLabel->text()));
    ui->portLineEdit->setText(QString::number(settings->port(ui->settingsNameLabel->text())));

    delete settings;
}

void ClientSettings::applySettings()
{
    IRealTimeSettings *settings = StaticCoreUtils::getRealTimeSettings();

    settings->setClientConnectionInfo(ui->settingsNameLabel->text(), ui->ipLineEdit->text(), ui->portLineEdit->text().toULongLong());

    delete settings;
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
    IRealTimeSettings *settings = StaticCoreUtils::getRealTimeSettings();

    ui->ipLineEdit->setText(settings->defaultIp(ui->settingsNameLabel->text()));
    ui->portLineEdit->setText(QString::number(settings->defaultPort(ui->settingsNameLabel->text())));

    delete settings;
}

void ClientSettings::buttonClicked(QAbstractButton *button)
{
    if(ui->buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole)
    {
        applySettings();
    }

    else if(ui->buttonBox->buttonRole(button) == QDialogButtonBox::ResetRole)
    {
        showDefaultSettings();
    }
}
