#include "recordsselectiondialog.h"

RecordsSelectionDialog::RecordsSelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordsSelectionDialog)
{
    ui->setupUi(this);
}

void RecordsSelectionDialog::getRecordsNumbers(int &fromRecord, int &toRecord)
{
    fromRecord = ui->fromRecordEdit->text().toInt();
    toRecord = ui->toRecordEdit->text().toInt();
}

RecordsSelectionDialog::~RecordsSelectionDialog()
{
    delete ui;
}
