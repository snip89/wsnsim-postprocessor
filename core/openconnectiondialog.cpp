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
    StaticCoreUtils::setPublicApplicationInfo();

    QSettings settings;
    QStringList clients = settings.value("Main/ClientsList").value<QStringList>();

    ui->comboBox->addItems(clients);
}

OpenConnectionDialog::~OpenConnectionDialog()
{
    delete ui;
}
