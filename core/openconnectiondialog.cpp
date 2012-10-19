#include "openconnectiondialog.h"

OpenConnectionDialog::OpenConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenConnectionDialog)
{
    ui->setupUi(this);

    ui->comboBox->addItem(tr("Sniffer"));
}

QString OpenConnectionDialog::getConnectionType()
{
    return ui->comboBox->currentText();
}

OpenConnectionDialog::~OpenConnectionDialog()
{
    delete ui;
}
