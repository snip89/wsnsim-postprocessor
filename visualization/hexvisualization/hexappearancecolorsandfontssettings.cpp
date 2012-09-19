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
    itemMainText->setFont(settings.value("Hex visualization/Appearance/Colors and Fonts/Font").value<QFont>());
    itemCurrentLine->setFont(settings.value("Hex visualization/Appearance/Colors and Fonts/Font").value<QFont>());
    // ui->fontListWidget->setCurrentFont(settings.value("Hex visualization/Appearance/Colors and Fonts/Font").value<QFont>());
    // updatePreviewText();

    // updateColorsList();
}

void HexAppearanceColorsAndFontsSettings::applySettings()
{
    settings.setValue("Hex visualization/Appearance/Colors and Fonts/Font", itemMainText->font());
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

    ui->fontListWidget->addItem(itemMainText);
    ui->fontListWidget->addItem(itemCurrentLine);

    ui->fontListWidget->setCurrentItem(itemMainText);
}

void HexAppearanceColorsAndFontsSettings::deleteListWidgetItems()
{
    delete itemMainText;
    delete itemCurrentLine;
}

/*void HexAppearanceColorsAndFontsSettings::updatePreviewText()
{
    ui->previewTextEdit->clear();
    ui->previewTextEdit->setText(tr("This is font preview text"));
}*/

void HexAppearanceColorsAndFontsSettings::updateColorsList()
{
}

void HexAppearanceColorsAndFontsSettings::showDefaultSettings()
{
    itemMainText->setFont(settings.value("Defaults/Hex visualization/Appearance/Colors and Fonts/Font").value<QFont>());
    itemCurrentLine->setFont(settings.value("Defaults/Hex visualization/Appearance/Colors and Fonts/Font").value<QFont>());
    // updatePreviewText();
}

void HexAppearanceColorsAndFontsSettings::buttonClicked(QAbstractButton *button)
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

void HexAppearanceColorsAndFontsSettings::buttonChangeFontClicked()
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
