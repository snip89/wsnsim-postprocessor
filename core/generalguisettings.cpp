#include "generalguisettings.h"
#include "ui_generalguisettings.h"

GeneralGuiSettings::GeneralGuiSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GeneralGuiSettings)
{
	ui->setupUi(this);

	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

void GeneralGuiSettings::setSettingsName(QString name)
{
    ui->settingsNameLabel->setText(name);
}

void GeneralGuiSettings::showCurrentSettings()
{
    ui->recentNumberSpinBox->setValue(settings.value("General/Gui/Recent_number").toInt());
}

void GeneralGuiSettings::applySettings()
{
    settings.setValue("General/Gui/Recent_number", ui->recentNumberSpinBox->text());
}

GeneralGuiSettings::~GeneralGuiSettings()
{
}

void GeneralGuiSettings::showDefaultSettings()
{
    ui->recentNumberSpinBox->setValue(settings.value("Defaults/General/Gui/Recent_number").toInt());
}

void GeneralGuiSettings::buttonClicked(QAbstractButton *button)
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
