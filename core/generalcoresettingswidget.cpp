#include "generalcoresettingswidget.h"

GeneralCoreSettingsWidget::GeneralCoreSettingsWidget(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

void GeneralCoreSettingsWidget::setSettingsName(QString name)
{
    ui.settingsNameLabel->setText(name);
}

void GeneralCoreSettingsWidget::showCurrentSettings()
{
    ui.memoryUsageSpinBox->setValue(settings.value("General/Core/Memory_usage").toInt());
    ui.blockSizeSpinBox->setValue(settings.value("General/Core/Block_size").toInt());
}

void GeneralCoreSettingsWidget::applySettings()
{
    settings.setValue("General/Core/Memory_usage", ui.memoryUsageSpinBox->text());
    settings.setValue("General/Core/Block_size", ui.blockSizeSpinBox->text());
}

GeneralCoreSettingsWidget::~GeneralCoreSettingsWidget()
{

}

void GeneralCoreSettingsWidget::showDefaultSettings()
{
    ui.memoryUsageSpinBox->setValue(settings.value("Defaults/General/Core/Memory_usage").toInt());
    ui.blockSizeSpinBox->setValue(settings.value("Defaults/General/Core/Block_size").toInt());
}

void GeneralCoreSettingsWidget::buttonClicked(QAbstractButton *button)
{
    if(button->text() == "Apply")
    {
        applySettings();
    }

    else if(button->text() == "Restore Defaults")
        showDefaultSettings();
}
