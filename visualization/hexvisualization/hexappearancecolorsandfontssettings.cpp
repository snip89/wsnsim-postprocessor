#include "hexappearancecolorsandfontssettings.h"

HexAppearanceColorsAndFontsSettings::HexAppearanceColorsAndFontsSettings(QWidget *parent) :
    AbstractTextAppearanceColorsAndFontsSettings(parent)
{
    createListWidgetItems();

    connect(ui->changeFontButton, SIGNAL(clicked()), this, SLOT(buttonChangeFontClicked()));

	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

void HexAppearanceColorsAndFontsSettings::setSettingsName(QString name)
{
    ui->settingsNameLabel->setText(name);
}

void HexAppearanceColorsAndFontsSettings::showCurrentSettings()
{
    ui->previewTextEdit->setCurrentFont(settings.value(tr("Hex visualization/Appearance/Colors and Fonts/Font")).value<QFont>());
    updatePreviewText();

    updateColorsList();
}

void HexAppearanceColorsAndFontsSettings::applySettings()
{
    settings.setValue(tr("Hex visualization/Appearance/Colors and Fonts/Font"), ui->previewTextEdit->currentFont());
}

QWidget *HexAppearanceColorsAndFontsSettings::getWidget()
{
    return this;
}

HexAppearanceColorsAndFontsSettings::~HexAppearanceColorsAndFontsSettings()
{
    deleteListWidgetItems();
}

void HexAppearanceColorsAndFontsSettings::createListWidgetItems()
{
    itemMainText = new QListWidgetItem(tr("main text"));
    itemMainText->setTextAlignment(Qt::AlignHCenter);

    itemCurrentLine = new QListWidgetItem(tr("current line"));
    itemCurrentLine->setTextAlignment(Qt::AlignHCenter);

    ui->colorsListWidget->addItem(itemMainText);
    ui->colorsListWidget->addItem(itemCurrentLine);

    ui->colorsListWidget->setCurrentItem(itemMainText);
}

void HexAppearanceColorsAndFontsSettings::deleteListWidgetItems()
{
    delete itemMainText;
    delete itemCurrentLine;
}

void HexAppearanceColorsAndFontsSettings::updatePreviewText()
{
    ui->previewTextEdit->clear();
    ui->previewTextEdit->setText(tr("This is font preview text"));
}

void HexAppearanceColorsAndFontsSettings::updateColorsList()
{
}

void HexAppearanceColorsAndFontsSettings::showDefaultSettings()
{
    ui->previewTextEdit->setCurrentFont(settings.value(tr("Defaults/Hex visualization/Appearance/Colors and Fonts/Font")).value<QFont>());
    updatePreviewText();
}

void HexAppearanceColorsAndFontsSettings::buttonClicked(QAbstractButton *button)
{
    if(button->text() == tr("Apply"))
    {
        applySettings();
    }

    else if(button->text() == tr("Restore Defaults"))
        showDefaultSettings();
}

void HexAppearanceColorsAndFontsSettings::buttonChangeFontClicked()
{
    QFont resultFont = ui->previewTextEdit->currentFont();

    QFontDialog *fontDialog = new QFontDialog(this);
//    fontDialog->setCurrentFont(resultFont);

    bool ok;
    resultFont = fontDialog->getFont(&ok, resultFont);

    if(ok)
    {
        ui->previewTextEdit->setCurrentFont(resultFont);
        updatePreviewText();
    }

    delete fontDialog;
}
