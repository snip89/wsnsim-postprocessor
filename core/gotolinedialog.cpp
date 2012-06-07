#include "gotolinedialog.h"

GoToLineDialog::GoToLineDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GoToLineDialog)
{
    ui->setupUi(this);
}

GoToLineDialog::~GoToLineDialog()
{
    delete ui;
}
