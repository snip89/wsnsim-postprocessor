#include "generalguisettingswidget.h"

GeneralGuiSettingsWidget::GeneralGuiSettingsWidget(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

void GeneralGuiSettingsWidget::setSettingsName(QString name)
{
    ui.settingsNameLabel->setText(name);
}

void GeneralGuiSettingsWidget::showCurrentSettings()
{
    ui.recentNumberSpinBox->setValue(settings.value("General/Gui/Recent_number").toInt());
}

void GeneralGuiSettingsWidget::applySettings()
{
    settings.setValue("General/Gui/Recent_number", ui.recentNumberSpinBox->text());
}

GeneralGuiSettingsWidget::~GeneralGuiSettingsWidget()
{
}

void GeneralGuiSettingsWidget::showDefaultSettings()
{
    ui.recentNumberSpinBox->setValue(settings.value("Defaults/General/Gui/Recent_number").toInt());
}

void GeneralGuiSettingsWidget::buttonClicked(QAbstractButton *button)
{
    if(button->text() == "Apply")
        applySettings();

    else if(button->text() == "Restore Defaults")
        showDefaultSettings();
}
