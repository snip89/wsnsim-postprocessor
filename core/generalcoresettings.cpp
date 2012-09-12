#include "generalcoresettings.h"
#include "ui_generalcoresettings.h"

GeneralCoreSettings::GeneralCoreSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GeneralCoreSettings)
{
	ui->setupUi(this);

	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

void GeneralCoreSettings::setSettingsName(QString name)
{
    ui->settingsNameLabel->setText(name);
}

void GeneralCoreSettings::showCurrentSettings()
{
    ui->memoryUsageSpinBox->setValue(settings.value(tr("General/Core/Memory_usage")).toInt());
    ui->blockSizeSpinBox->setValue(settings.value(tr("General/Core/Block_size")).toInt());
}

void GeneralCoreSettings::applySettings()
{
    settings.setValue(tr("General/Core/Memory_usage"), ui->memoryUsageSpinBox->text());
    settings.setValue(tr("General/Core/Block_size"), ui->blockSizeSpinBox->text());
}

GeneralCoreSettings::~GeneralCoreSettings()
{
    delete ui;
}

void GeneralCoreSettings::showDefaultSettings()
{
    ui->memoryUsageSpinBox->setValue(settings.value(tr("Defaults/General/Core/Memory_usage")).toInt());
    ui->blockSizeSpinBox->setValue(settings.value(tr("Defaults/General/Core/Block_size")).toInt());
}

void GeneralCoreSettings::buttonClicked(QAbstractButton *button)
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
