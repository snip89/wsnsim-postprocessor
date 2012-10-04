#include "sniffercoresettings.h"
#include "ui_sniffercoresettings.h"

SnifferCoreSettings::SnifferCoreSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SnifferCoreSettings)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

void SnifferCoreSettings::setSettingsName(QString name)
{
    ui->settingsNameLabel->setText(name);
}

void SnifferCoreSettings::showCurrentSettings()
{

}

void SnifferCoreSettings::applySettings()
{

}

SnifferCoreSettings::~SnifferCoreSettings()
{
    delete ui;
}

void SnifferCoreSettings::showDefaultSettings()
{

}

void SnifferCoreSettings::buttonClicked(QAbstractButton *button)
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