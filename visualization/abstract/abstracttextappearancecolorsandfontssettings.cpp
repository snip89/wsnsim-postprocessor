#include "abstracttextappearancecolorsandfontssettings.h"

AbstractTextAppearanceColorsAndFontsSettings::AbstractTextAppearanceColorsAndFontsSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbstractTextAppearanceColorsAndFontsSettings)
{
    ui->setupUi(this);
}

AbstractTextAppearanceColorsAndFontsSettings::~AbstractTextAppearanceColorsAndFontsSettings()
{
    delete ui;
}
