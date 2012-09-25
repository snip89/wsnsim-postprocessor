#include "abstracttextguisettings.h"

AbstractTextGuiSettings::AbstractTextGuiSettings(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::AbstractTextGuiSettings)
{
	ui->setupUi(this);
}

AbstractTextGuiSettings::~AbstractTextGuiSettings()
{
    delete ui;
}