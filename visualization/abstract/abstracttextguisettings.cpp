#include "abstracttextguisettings.h"

AbstractTextGuiSettings::AbstractTextGuiSettings(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::AbstractTextGuiSettings)
{
	ui->setupUi(this);

	ui->incrementLineEdit->setValidator(new QIntValidator(1, qPow(2, sizeof(int)*8) / 2));
}

AbstractTextGuiSettings::~AbstractTextGuiSettings()
{
    delete ui;
}