#include "textguisettings.h"

TextGuiSettings::TextGuiSettings(QWidget *parent) :
	AbstractTextGuiSettings(parent)
{
	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

void TextGuiSettings::setSettingsName(QString name)
{
	ui->settingsNameLabel->setText(name);
}

void TextGuiSettings::showCurrentSettings()
{
    ui->incrementSpinBox->setValue(settings.value("Text visualization/Gui/Increment").value<int>());    
}

void TextGuiSettings::applySettings()
{
    settings.setValue("Text visualization/Gui/Increment", ui->incrementSpinBox->value());    
}

TextGuiSettings::~TextGuiSettings()
{
}

void TextGuiSettings::showDefaultSettings()
{
    ui->incrementSpinBox->setValue(settings.value("Defaults/Text visualization/Gui/Increment").value<int>());    
}

void TextGuiSettings::buttonClicked(QAbstractButton *button)
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