#include "snifferguisettings.h"
#include "ui_snifferguisettings.h"

SnifferGuiSettings::SnifferGuiSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SnifferGuiSettings)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

void SnifferGuiSettings::setSettingsName(QString name)
{
    ui->settingsNameLabel->setText(name);
}

void SnifferGuiSettings::showCurrentSettings()
{

}

void SnifferGuiSettings::applySettings()
{

}

SnifferGuiSettings::~SnifferGuiSettings()
{
    delete ui;
}

void SnifferGuiSettings::showDefaultSettings()
{

}

void SnifferGuiSettings::buttonClicked(QAbstractButton *button)
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