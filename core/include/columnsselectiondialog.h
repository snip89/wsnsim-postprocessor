#ifndef COLUMNSSELECTIONDIALOG_H
#define COLUMNSSELECTIONDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QStringList>
#include <QDebug>

#include "ui_columnsselectiondialog.h"

namespace Ui {
    class ColumnsSelectionDialog;
}

class ColumnsSelectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ColumnsSelectionDialog(QWidget *parent = 0);
    virtual ~ColumnsSelectionDialog();
private:
    QSettings settings;

    void initListWidget();

    Ui::ColumnsSelectionDialog *ui;

private slots:
    void buttonClicked(QAbstractButton *button);
};

#endif // COLUMNSSELECTIONDIALOG_H
