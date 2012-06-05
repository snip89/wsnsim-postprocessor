#include "hexappearancecolorsandfontssettings.h"

HexAppearanceColorsAndFontsSettings::HexAppearanceColorsAndFontsSettings(QWidget *parent) :
    AbstractTextAppearanceColorsAndFontsSettings(parent)
{
    connect(ui->changeFontButton, SIGNAL(clicked()), this, SLOT(buttonChangeFontClicked()));

	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

void HexAppearanceColorsAndFontsSettings::setSettingsName(QString name)
{
    ui->settingsNameLabel->setText(name);
}

void HexAppearanceColorsAndFontsSettings::showCurrentSettings()
{
    ui->previewTextEdit->setCurrentFont(settings.value("Hex visualization/Appearance/Colors and Fonts/Font").value<QFont>());
    updatePreviewText();
}

void HexAppearanceColorsAndFontsSettings::applySettings()
{
    settings.setValue("Hex visualization/Appearance/Colors and Fonts/Font", ui->previewTextEdit->currentFont());
}

QWidget *HexAppearanceColorsAndFontsSettings::getWidget()
{
    return this;
}

HexAppearanceColorsAndFontsSettings::~HexAppearanceColorsAndFontsSettings()
{
}

void HexAppearanceColorsAndFontsSettings::updatePreviewText()
{
    ui->previewTextEdit->clear();
    ui->previewTextEdit->setText("This is font preview text");
}

void HexAppearanceColorsAndFontsSettings::showDefaultSettings()
{
    ui->previewTextEdit->setCurrentFont(settings.value("Defaults/Hex visualization/Appearance/Colors and Fonts/Font").value<QFont>());
    updatePreviewText();
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

void HexAppearanceColorsAndFontsSettings::buttonChangeFontClicked()
{
    QFont resultFont = settings.value("Hex visualization/Appearance/Colors and Fonts/Font").value<QFont>();

    QFontDialog *fontDialog = new QFontDialog(this);

    bool ok;
    resultFont = fontDialog->getFont(&ok);

    if(ok)
    {
        ui->previewTextEdit->setCurrentFont(resultFont);
        updatePreviewText();
    }

    delete fontDialog;
}
