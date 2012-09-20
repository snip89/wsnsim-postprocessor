#include "hexappearancecolorsandfontssettings.h"

HexAppearanceColorsAndFontsSettings::HexAppearanceColorsAndFontsSettings(QWidget *parent) :
    AbstractTextAppearanceColorsAndFontsSettings(parent)
{
    createListWidgetItems();

    connect(ui->changeFontButton, SIGNAL(clicked()), this, SLOT(buttonChangeFontClicked()));
    connect(ui->foregroundPushButton, SIGNAL(clicked()), this, SLOT(buttonChangeForegroundClicked()));
    connect(ui->backgroundPushButton, SIGNAL(clicked()), this, SLOT(buttonChangeBackgroundClicked()));

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

    itemMainText->setBackground(settings.value("Hex visualization/Appearance/Colors and Fonts/Main_text_background").value<QColor>());
    itemCurrentLine->setBackground(settings.value("Hex visualization/Appearance/Colors and Fonts/Cursor_line_color").value<QColor>());

    itemMainText->setForeground(settings.value("Hex visualization/Appearance/Colors and Fonts/Main_text_foreground").value<QColor>());
    itemCurrentLine->setForeground(settings.value("Hex visualization/Appearance/Colors and Fonts/Cursor_line_font_color").value<QColor>());
}

void HexAppearanceColorsAndFontsSettings::applySettings()
{
    settings.setValue("Hex visualization/Appearance/Colors and Fonts/Font", itemMainText->font());

    settings.setValue("Hex visualization/Appearance/Colors and Fonts/Main_text_background", itemMainText->background().color());
    settings.setValue("Hex visualization/Appearance/Colors and Fonts/Cursor_line_color", itemCurrentLine->background().color());

    settings.setValue("Hex visualization/Appearance/Colors and Fonts/Main_text_foreground", itemMainText->foreground().color());
    settings.setValue("Hex visualization/Appearance/Colors and Fonts/Cursor_line_font_color", itemCurrentLine->foreground().color());
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

void HexAppearanceColorsAndFontsSettings::updatePreviewText()
{
    // ui->previewTextEdit->clear();
    // ui->previewTextEdit->setText(tr("This is font preview text"));
}

void HexAppearanceColorsAndFontsSettings::updateColorsList()
{
}

void HexAppearanceColorsAndFontsSettings::showDefaultSettings()
{
    itemMainText->setFont(settings.value("Defaults/Hex visualization/Appearance/Colors and Fonts/Font").value<QFont>());
    itemCurrentLine->setFont(settings.value("Defaults/Hex visualization/Appearance/Colors and Fonts/Font").value<QFont>());

    itemMainText->setBackground(settings.value("Defaults/Hex visualization/Appearance/Colors and Fonts/Main_text_background").value<QColor>());
    itemCurrentLine->setBackground(settings.value("Defaults/Hex visualization/Appearance/Colors and Fonts/Cursor_line_color").value<QColor>());

    itemMainText->setForeground(settings.value("Defaults/Hex visualization/Appearance/Colors and Fonts/Main_text_foreground").value<QColor>());
    itemCurrentLine->setForeground(settings.value("Defaults/Hex visualization/Appearance/Colors and Fonts/Cursor_line_font_color").value<QColor>());
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

    QFontDialog *fontDialog = new QFontDialog(resultFont, this);

    fontDialog->move(settings.value("Hidden/Gui/Font_dialog_pos").value<QPoint>());
    fontDialog->resize(settings.value("Hidden/Gui/Font_dialog_size").value<QSize>());

    if(fontDialog->exec())
    {
        resultFont = fontDialog->selectedFont();

        itemMainText->setFont(resultFont);
        itemCurrentLine->setFont(resultFont);
    }

    settings.setValue("Hidden/Gui/Font_dialog_pos", fontDialog->pos());
    settings.setValue("Hidden/Gui/Font_dialog_size", fontDialog->size());

    delete fontDialog;
}

void HexAppearanceColorsAndFontsSettings::buttonChangeForegroundClicked()
{
    if(ui->fontListWidget->currentItem() == itemMainText)
    {
        QColor resultColor = itemMainText->foreground().color();

        QColorDialog *colorDialog = new QColorDialog(resultColor, this);

        colorDialog->move(settings.value("Hidden/Gui/Color_dialog_pos").value<QPoint>());
        colorDialog->resize(settings.value("Hidden/Gui/Color_dialog_size").value<QSize>());

        if(colorDialog->exec())
        {
            resultColor = colorDialog->selectedColor();

            itemMainText->setForeground(resultColor);
        }

        settings.setValue("Hidden/Gui/Color_dialog_pos", colorDialog->pos());
        settings.setValue("Hidden/Gui/Color_dialog_size", colorDialog->size());

        delete colorDialog;
    }

    else if(ui->fontListWidget->currentItem() == itemCurrentLine)
    {
        QColor resultColor = itemCurrentLine->foreground().color();

        QColorDialog *colorDialog = new QColorDialog(resultColor, this);

        colorDialog->move(settings.value("Hidden/Gui/Color_dialog_pos").value<QPoint>());
        colorDialog->resize(settings.value("Hidden/Gui/Color_dialog_size").value<QSize>());

        if(colorDialog->exec())
        {
            resultColor = colorDialog->selectedColor();

            itemCurrentLine->setForeground(resultColor);
        }

        settings.setValue("Hidden/Gui/Color_dialog_pos", colorDialog->pos());
        settings.setValue("Hidden/Gui/Color_dialog_size", colorDialog->size());

        delete colorDialog;
    }
}

void HexAppearanceColorsAndFontsSettings::buttonChangeBackgroundClicked()
{
    if(ui->fontListWidget->currentItem() == itemMainText)
    {
        QColor resultColor = itemMainText->background().color();

        QColorDialog *colorDialog = new QColorDialog(resultColor, this);

        colorDialog->move(settings.value("Hidden/Gui/Color_dialog_pos").value<QPoint>());
        colorDialog->resize(settings.value("Hidden/Gui/Color_dialog_size").value<QSize>());

        if(colorDialog->exec())
        {
            resultColor = colorDialog->selectedColor();

            itemMainText->setForeground(resultColor);
        }

        settings.setValue("Hidden/Gui/Color_dialog_pos", colorDialog->pos());
        settings.setValue("Hidden/Gui/Color_dialog_size", colorDialog->size());

        delete colorDialog;
    }

    else if(ui->fontListWidget->currentItem() == itemCurrentLine)
    {
        QColor resultColor = itemCurrentLine->background().color();

        QColorDialog *colorDialog = new QColorDialog(resultColor, this);

        colorDialog->move(settings.value("Hidden/Gui/Color_dialog_pos").value<QPoint>());
        colorDialog->resize(settings.value("Hidden/Gui/Color_dialog_size").value<QSize>());

        if(colorDialog->exec())
        {
            resultColor = colorDialog->selectedColor();

            itemCurrentLine->setForeground(resultColor);
        }

        settings.setValue("Hidden/Gui/Color_dialog_pos", colorDialog->pos());
        settings.setValue("Hidden/Gui/Color_dialog_size", colorDialog->size());

        delete colorDialog;
    }
}
