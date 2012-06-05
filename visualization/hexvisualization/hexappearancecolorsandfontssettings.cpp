#include "hexappearancecolorsandfontssettings.h"

HexAppearanceColorsAndFontsSettings::HexAppearanceColorsAndFontsSettings(QWidget *parent) :
    AbstractTextAppearanceColorsAndFontsSettings(parent)
{
}

void HexAppearanceColorsAndFontsSettings::setSettingsName(QString name)
{
    ui->settingsNameLabel->setText(name);
}

void HexAppearanceColorsAndFontsSettings::showCurrentSettings()
{
    ui->fontComboBox->setCurrentFont(settings.value("Hex visualization/Appearance/Colors and Fonts/Font").value<QFont>());
}

void HexAppearanceColorsAndFontsSettings::applySettings()
{
}

QWidget *HexAppearanceColorsAndFontsSettings::getWidget()
{
    return this;
}

HexAppearanceColorsAndFontsSettings::~HexAppearanceColorsAndFontsSettings()
{
}

void HexAppearanceColorsAndFontsSettings::showDefaultSettings()
{
}

void HexAppearanceColorsAndFontsSettings::buttonClicked(QAbstractButton *button)
{
    if(button->text() == "Apply")
    {
        applySettings();
    }

    else if(button->text() == "Restore Defaults")
        showDefaultSettings();
}
