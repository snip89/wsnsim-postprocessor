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
    QDir::setCurrent(QApplication::applicationDirPath());

    QLibrary globalSettings("./globalSettings");

    typedef IHostRealTimeSettings*(*getRealTimeSettings) ();
    getRealTimeSettings func = (getRealTimeSettings) globalSettings.resolve("getRealTimeSettings");

    IHostRealTimeSettings* settings = func();

    foreach(QString client, settings->clients())
    {
        ui->comboBox->addItem(client);
    }
}

OpenConnectionDialog::~OpenConnectionDialog()
{
    delete ui;
}
