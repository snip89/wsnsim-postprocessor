#include "hexguisettings.h"

HexGuiSettings::HexGuiSettings(QWidget *parent) :
	AbstractTextGuiSettings(parent)
{
	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));

    ui->lineWrapComboBox->addItem(tr("On"));
    ui->lineWrapComboBox->addItem(tr("Off"));
}

void HexGuiSettings::setSettingsName(QString name)
{
	ui->settingsNameLabel->setText(name);
}

void HexGuiSettings::showCurrentSettings()
{
    ui->incrementSpinBox->setValue(settings.value("Hex visualization/Gui/Increment").value<int>());

    bool lineWrapMode = settings.value("Hex visualization/Gui/LineWrapMode").value<bool>();

    if(lineWrapMode)
        ui->lineWrapComboBox->setCurrentIndex(0);
    else
        ui->lineWrapComboBox->setCurrentIndex(1);
}

void HexGuiSettings::applySettings()
{
    settings.setValue("Hex visualization/Gui/Increment", ui->incrementSpinBox->value());

    if(ui->lineWrapComboBox->currentIndex() == 0)
        settings.setValue("Hex visualization/Gui/LineWrapMode", 1);
    else
        settings.setValue("Hex visualization/Gui/LineWrapMode", 0);
}

HexGuiSettings::~HexGuiSettings()
{
}

void HexGuiSettings::showDefaultSettings()
{
    ui->incrementSpinBox->setValue(settings.value("Defaults/Hex visualization/Gui/Increment").value<int>());

    bool lineWrapMode = settings.value("Defaults/Hex visualization/Gui/LineWrapMode").value<bool>();

    if(lineWrapMode)
        ui->lineWrapComboBox->setCurrentIndex(0);
    else
        ui->lineWrapComboBox->setCurrentIndex(1);    
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