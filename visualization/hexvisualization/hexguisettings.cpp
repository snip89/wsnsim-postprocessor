#include "hexguisettings.h"

HexGuiSettings::HexGuiSettings(QWidget *parent) :
	AbstractTextGuiSettings(parent)
{
	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

void HexGuiSettings::setSettingsName(QString name)
{
	ui->settingsNameLabel->setText(name);
}

void HexGuiSettings::showCurrentSettings()
{
    ui->incrementSpinBox->setValue(settings.value("Hex visualization/Gui/Increment").value<int>());
}

void HexGuiSettings::applySettings()
{
    settings.setValue("Hex visualization/Gui/Increment", ui->incrementSpinBox->value());
}

HexGuiSettings::~HexGuiSettings()
{
}

void HexGuiSettings::showDefaultSettings()
{
    ui->incrementSpinBox->setValue(settings.value("Defaults/Hex visualization/Gui/Increment").value<int>());
}

void HexGuiSettings::buttonClicked(QAbstractButton *button)
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