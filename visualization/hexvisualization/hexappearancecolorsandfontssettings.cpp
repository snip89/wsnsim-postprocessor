#include "hexappearancecolorsandfontssettings.h"

HexAppearanceColorsAndFontsSettings::HexAppearanceColorsAndFontsSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HexAppearanceColorsAndFontsSettings)
{
    ui->setupUi(this);
}

HexAppearanceColorsAndFontsSettings::~HexAppearanceColorsAndFontsSettings()
{
    delete ui;
}
