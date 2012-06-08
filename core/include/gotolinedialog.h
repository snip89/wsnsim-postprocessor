/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef GOTOLINEDIALOG_H
#define GOTOLINEDIALOG_H

#include <QDialog>
#include <QString>
#include <QValidator>

#include "ui_gotolinedialog.h"

namespace Ui {
    class GoToLineDialog;
}

class GoToLineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GoToLineDialog(QWidget *parent = 0);

    qint64 lineNumber();

    virtual ~GoToLineDialog();

private:
    Ui::GoToLineDialog *ui;
};

#endif // GOTOLINEDIALOG_H
