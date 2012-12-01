#ifndef FORMATACCEPTINGDIALOG_H
#define FORMATACCEPTINGDIALOG_H

#include <QDialog>
#include <QString>
#include <QMap>

#include "ui_formatacceptingdialog.h"
#include "projectParams.h"
#include "project.h"

namespace Ui {
    class FormatAcceptingDialog;
}

class FormatAcceptingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FormatAcceptingDialog(Project *project, QString type, QWidget *parent = 0);
    AttrInfo getArgument();
    virtual ~FormatAcceptingDialog();
private:
    Project *project;

    QString name;

    void initComboBox();

    Ui::FormatAcceptingDialog *ui;
private slots:
    void updateArgComboBox(int index);
};

#endif // FORMATACCEPTINGDIALOG_H
