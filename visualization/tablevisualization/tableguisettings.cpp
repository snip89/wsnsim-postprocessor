#include "tableguisettings.h"

TableGuiSettings::TableGuiSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableGuiSettings)
{
    ui->setupUi(this);

    ui->incrementLineEdit->setValidator(new QIntValidator(1, qPow(2, sizeof(int)*8) / 2));

    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

void TableGuiSettings::setSettingsName(QString name)
{
    ui->settingsNameLabel->setText(name);
}

void TableGuiSettings::showCurrentSettings()
{
    ui->incrementLineEdit->setText(QString::number(settings.value("Table visualization/Gui/Increment").value<int>()));
}

void TableGuiSettings::applySettings()
{
    settings.setValue("Table visualization/Gui/Increment", ui->incrementLineEdit->text().toInt());
}

TableGuiSettings::~TableGuiSettings()
{
}

void TableGuiSettings::showDefaultSettings()
{
    ui->incrementLineEdit->setText(QString::number(settings.value("Defaults/Table visualization/Gui/Increment").value<int>()));
}

void TableGuiSettings::buttonClicked(QAbstractButton *button)
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
