#include "textappearancecolorsandfontssettings.h"

TextAppearanceColorsAndFontsSettings::TextAppearanceColorsAndFontsSettings(QWidget *parent) :
    AbstractTextAppearanceColorsAndFontsSettings(parent)
{
    createListWidgetItems();

    connect(ui->changeFontButton, SIGNAL(clicked()), this, SLOT(buttonChangeFontClicked()));

    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

void TextAppearanceColorsAndFontsSettings::setSettingsName(QString name)
{
    ui->settingsNameLabel->setText(name);
}

void TextAppearanceColorsAndFontsSettings::showCurrentSettings()
{
    itemMainText->setFont(settings.value("Text visualization/Appearance/Colors and Fonts/Font").value<QFont>());
    itemCurrentLine->setFont(settings.value("Text visualization/Appearance/Colors and Fonts/Font").value<QFont>());
 /*   ui->previewTextEdit->setCurrentFont(settings.value("Text visualization/Appearance/Colors and Fonts/Font").value<QFont>());
    updatePreviewText();*/
}

void TextAppearanceColorsAndFontsSettings::applySettings()
{
    settings.setValue("Text visualization/Appearance/Colors and Fonts/Font", itemMainText->font());
    // settings.setValue("Text visualization/Appearance/Colors and Fonts/Font", ui->previewTextEdit->currentFont());
}

QWidget *TextAppearanceColorsAndFontsSettings::getWidget()
{
    return this;
}

TextAppearanceColorsAndFontsSettings::~TextAppearanceColorsAndFontsSettings()
{
    deleteListWidgetItems();
}

void TextAppearanceColorsAndFontsSettings::createListWidgetItems()
{
    itemMainText = new QListWidgetItem(tr("main text"));
    itemMainText->setTextAlignment(Qt::AlignHCenter);

    itemCurrentLine = new QListWidgetItem(tr("current line"));
    itemCurrentLine->setTextAlignment(Qt::AlignHCenter);

    ui->fontListWidget->addItem(itemMainText);
    ui->fontListWidget->addItem(itemCurrentLine);

    ui->fontListWidget->setCurrentItem(itemMainText);
}

void TextAppearanceColorsAndFontsSettings::deleteListWidgetItems()
{
    delete itemMainText;
    delete itemCurrentLine;
}

void TextAppearanceColorsAndFontsSettings::updatePreviewText()
{
 /*   ui->previewTextEdit->clear();
    ui->previewTextEdit->setText(tr("This is font preview text"));*/
}

void TextAppearanceColorsAndFontsSettings::showDefaultSettings()
{
       itemMainText->setFont(settings.value("Defaults/Text visualization/Appearance/Colors and Fonts/Font").value<QFont>());
    itemCurrentLine->setFont(settings.value("Defaults/Text visualization/Appearance/Colors and Fonts/Font").value<QFont>());
 /*   ui->previewTextEdit->setCurrentFont(settings.value("Defaults/Text visualization/Appearance/Colors and Fonts/Font").value<QFont>());
    updatePreviewText();*/
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
    QFont resultFont = itemMainText->font();

    QFontDialog *fontDialog = new QFontDialog(this);
//    fontDialog->setCurrentFont(resultFont);

    bool ok;
    resultFont = fontDialog->getFont(&ok, resultFont);

    if(ok)
    {
        itemMainText->setFont(resultFont);
        itemCurrentLine->setFont(resultFont);
        // updatePreviewText();
    }

    delete fontDialog;
}
