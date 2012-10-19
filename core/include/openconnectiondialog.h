#ifndef OPENCONNECTIONDIALOG_H
#define OPENCONNECTIONDIALOG_H

#include "ui_openconnectiondialog.h"

namespace Ui {
    class OpenConnectionDialog;
}

class OpenConnectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit OpenConnectionDialog(QWidget *parent = 0);
    QString getConnectionType();
    virtual ~OpenConnectionDialog();
private:
    Ui::OpenConnectionDialog *ui;
};

#endif // OPENCONNECTIONDIALOG_H
