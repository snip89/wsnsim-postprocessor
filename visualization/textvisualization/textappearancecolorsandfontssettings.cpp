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
    ui->previewTextEdit->setCurrentFont(settings.value("Text visualization/Appearance/Colors and Fonts/Font").value<QFont>());
    updatePreviewText();
}

void TextAppearanceColorsAndFontsSettings::applySettings()
{
    settings.setValue("Text visualization/Appearance/Colors and Fonts/Font", ui->previewTextEdit->currentFont());
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
    ui->previewTextEdit->setText("This is font preview text");
}

void TextAppearanceColorsAndFontsSettings::showDefaultSettings()
{
    ui->previewTextEdit->setCurrentFont(settings.value("Defaults/Text visualization/Appearance/Colors and Fonts/Font").value<QFont>());
    updatePreviewText();
}

void TextAppearanceColorsAndFontsSettings::buttonClicked(QAbstractButton *button)
{
    if(button->text() == "Apply")
    {
        applySettings();
    }

    else if(button->text() == "Restore Defaults")
        showDefaultSettings();
}

void TextAppearanceColorsAndFontsSettings::buttonChangeFontClicked()
{
    QFont resultFont = settings.value("Text visualization/Appearance/Colors and Fonts/Font").value<QFont>();

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
