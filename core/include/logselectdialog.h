/*
 * logselectdialog.h
 *
 *  Created on: May 8, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#ifndef LOGSELECTDIALOG_H_
#define LOGSELECTDIALOG_H_

#include <QDialog>
#include <QWidget>

#include <QDebug>

#include "project.h"

namespace Ui {
    class LogSelectDialog;
}

class LogSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogSelectDialog(QWidget *parent, Project *logicProject);
    static int selectedLogId(QWidget *parent = 0, Project *logicProject = 0);
    int logId();
    virtual ~LogSelectDialog();
private:
    Project *project;

    Ui::LogSelectDialog *ui;

    void initSettings();
};

#endif /* LOGSELECTDIALOG_H_ */
