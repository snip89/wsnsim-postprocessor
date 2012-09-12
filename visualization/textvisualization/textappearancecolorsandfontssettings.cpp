#include "textappearancecolorsandfontssettings.h"

TextAppearanceColorsAndFontsSettings::TextAppearanceColorsAndFontsSettings(QWidget *parent) :
    AbstractTextAppearanceColorsAndFontsSettings(parent)
{
    connect(ui->changeFontButton, SIGNAL(clicked()), this, SLOT(buttonChangeFontClicked()));

	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

void TextAppearanceColorsAndFontsSettings::setSettingsName(QString name)
{
    ui->settingsNameLabel->setText(name);
}

void TextAppearanceColorsAndFontsSettings::showCurrentSettings()
{
    ui->previewTextEdit->setCurrentFont(settings.value(tr("Text visualization/Appearance/Colors and Fonts/Font")).value<QFont>());
    updatePreviewText();
}

void TextAppearanceColorsAndFontsSettings::applySettings()
{
    settings.setValue(tr("Text visualization/Appearance/Colors and Fonts/Font"), ui->previewTextEdit->currentFont());
}

QWidget *TextAppearanceColorsAndFontsSettings::getWidget()
{
    return this;
}

TextAppearanceColorsAndFontsSettings::~TextAppearanceColorsAndFontsSettings()
{
}

void TextAppearanceColorsAndFontsSettings::updatePreviewText()
{
    ui->previewTextEdit->clear();
    ui->previewTextEdit->setText(tr("This is font preview text"));
}

void TextAppearanceColorsAndFontsSettings::showDefaultSettings()
{
    ui->previewTextEdit->setCurrentFont(settings.value(tr("Defaults/Text visualization/Appearance/Colors and Fonts/Font")).value<QFont>());
    updatePreviewText();
}

void TextAppearanceColorsAndFontsSettings::buttonClicked(QAbstractButton *button)
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

void TextAppearanceColorsAndFontsSettings::buttonChangeFontClicked()
{
    QFont resultFont = settings.value(tr("Text visualization/Appearance/Colors and Fonts/Font")).value<QFont>();

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
