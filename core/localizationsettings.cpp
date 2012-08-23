#include "localizationsettings.h"
#include "ui_localizationsettings.h"

LocalizationSettings::LocalizationSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LocalizationSettings)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

void LocalizationSettings::setSettingsName(QString name)
{
    ui->settingsNameLabel->setText(name);
}

void LocalizationSettings::showCurrentSettings()
{   
}

void LocalizationSettings::applySettings()
{
}

LocalizationSettings::~LocalizationSettings()
{
    delete ui;
}

void LocalizationSettings::showDefaultSettings()
{
}

void LocalizationSettings::buttonClicked(QAbstractButton *button)
{
}
