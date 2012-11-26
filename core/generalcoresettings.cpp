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
    ui->memoryUsageSpinBox->setValue(settings.value("General/Core/Memory_usage").toInt());
    ui->blockSizeSpinBox->setValue(settings.value("General/Core/Block_size").toInt());

    if(settings.value("General/Core/Save_index").value<bool>())
        ui->saveIndexCheckBox->setCheckState(Qt::Checked);
    else
        ui->saveIndexCheckBox->setCheckState(Qt::Unchecked);
}

void GeneralCoreSettings::applySettings()
{
    settings.setValue("General/Core/Memory_usage", ui->memoryUsageSpinBox->text());
    settings.setValue("General/Core/Block_size", ui->blockSizeSpinBox->text());

    if(ui->saveIndexCheckBox->checkState())
        settings.setValue("General/Core/Save_index", true);

    else
        settings.setValue("General/Core/Save_index", false);
}

GeneralCoreSettings::~GeneralCoreSettings()
{
    delete ui;
}

void GeneralCoreSettings::showDefaultSettings()
{
    ui->memoryUsageSpinBox->setValue(settings.value("Defaults/General/Core/Memory_usage").toInt());
    ui->blockSizeSpinBox->setValue(settings.value("Defaults/General/Core/Block_size").toInt());

    if(settings.value("Defaults/General/Core/Save_index").value<bool>())
        ui->saveIndexCheckBox->setCheckState(Qt::Checked);
    else
        ui->saveIndexCheckBox->setCheckState(Qt::Checked);
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
