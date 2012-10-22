#include "openconnectiondialog.h"

OpenConnectionDialog::OpenConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenConnectionDialog)
{
    ui->setupUi(this);

    initComboBox();
}

QString OpenConnectionDialog::getConnectionType()
{
    return ui->comboBox->currentText();
}

void OpenConnectionDialog::initComboBox()
{
    IHostRealTimeSettings* settings = StaticCoreUtils::getHostRealTimeSettings();

    foreach(QString client, settings->clients())
    {
        ui->comboBox->addItem(client);
    }

    delete settings;
}

OpenConnectionDialog::~OpenConnectionDialog()
{
    delete ui;
}
