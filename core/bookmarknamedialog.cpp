#include "bookmarknamedialog.h"

BookmarkNameDialog::BookmarkNameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookmarkNameDialog)
{
    ui->setupUi(this);
}

QString BookmarkNameDialog::getName()
{
    return ui->lineEdit->text();
}

BookmarkNameDialog::~BookmarkNameDialog()
{
    delete ui;
}
