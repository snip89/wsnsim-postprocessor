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
