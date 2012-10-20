#ifndef OPENCONNECTIONDIALOG_H
#define OPENCONNECTIONDIALOG_H

#include <QSettings>
#include <QStringList>
#include <QLibrary>
#include <QDir>

#include "ui_openconnectiondialog.h"
#include "staticcoreutils.h"
#include "ihostrealtimesettings.h"

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
    void initComboBox();

    Ui::OpenConnectionDialog *ui;
};

#endif // OPENCONNECTIONDIALOG_H
