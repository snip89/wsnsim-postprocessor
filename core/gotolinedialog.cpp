#include "gotolinedialog.h"

GoToLineDialog::GoToLineDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GoToLineDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setValidator(new QIntValidator(this));
}

qint64 GoToLineDialog::lineNumber()
{
    return ui->lineEdit->text().toInt();
}

GoToLineDialog::~GoToLineDialog()
{
    delete ui;
}
