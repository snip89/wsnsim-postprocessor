#ifndef RECORDSSELECTIONDIALOG_H
#define RECORDSSELECTIONDIALOG_H

#include <QDialog>

#include "ui_recordsselectiondialog.h"

namespace Ui {
    class RecordsSelectionDialog;
}

class RecordsSelectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RecordsSelectionDialog(QWidget *parent);
    void getRecordsNumbers(int &fromRecord, int &toRecord);
    virtual ~RecordsSelectionDialog();
private:
    Ui::RecordsSelectionDialog *ui;
};

#endif // RECORDSSELECTIONDIALOG_H
