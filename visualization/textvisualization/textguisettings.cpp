#include "textguisettings.h"

TextGuiSettings::TextGuiSettings(QWidget *parent) :
	AbstractTextGuiSettings(parent)
{
	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));

    ui->lineWrapComboBox->addItem(tr("On"));
    ui->lineWrapComboBox->addItem(tr("Off"));    
}

void TextGuiSettings::setSettingsName(QString name)
{
	ui->settingsNameLabel->setText(name);
}

void TextGuiSettings::showCurrentSettings()
{
    ui->incrementLineEdit->setText(QString::number(settings.value("Text visualization/Gui/Increment").value<int>()));

    bool lineWrapMode = settings.value("Text visualization/Gui/LineWrapMode").value<bool>();

    if(lineWrapMode)
        ui->lineWrapComboBox->setCurrentIndex(0);
    else
        ui->lineWrapComboBox->setCurrentIndex(1);
}

void TextGuiSettings::applySettings()
{
        settings.setValue("Text visualization/Gui/Increment", ui->incrementLineEdit->text().toInt());

    if(ui->lineWrapComboBox->currentIndex() == 0)
        settings.setValue("Text visualization/Gui/LineWrapMode", 1);
    else
        settings.setValue("Text visualization/Gui/LineWrapMode", 0);    
}

TextGuiSettings::~TextGuiSettings()
{
}

void TextGuiSettings::showDefaultSettings()
{
    ui->incrementLineEdit->setText(QString::number(settings.value("Defaults/Text visualization/Gui/Increment").value<int>()));

    bool lineWrapMode = settings.value("Defaults/Text visualization/Gui/LineWrapMode").value<bool>();

    if(lineWrapMode)
        ui->lineWrapComboBox->setCurrentIndex(0);
    else
        ui->lineWrapComboBox->setCurrentIndex(1);        
}

void TextGuiSettings::buttonClicked(QAbstractButton *button)
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
