#include "emptysettings.h"

EmptySettings::EmptySettings(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
}

void EmptySettings::setSettingsName(QString name)
{
    ui.settingsNameLabel->setText(name);
}

EmptySettings::~EmptySettings()
{

}
