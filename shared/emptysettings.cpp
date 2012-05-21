#include "emptysettings.h"
#include "ui_emptysettings.h"

EmptySettings::EmptySettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EmptySettings)
{
	ui->setupUi(this);
}

void EmptySettings::setSettingsName(QString name)
{
    ui->settingsNameLabel->setText(name);
}

EmptySettings::~EmptySettings()
{
    delete ui;
}
