/*
 * logselectdialog.cpp
 *
 *  Created on: May 8, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#include "logselectdialog.h"
#include "ui_logselectdialog.h"

LogSelectDialog::LogSelectDialog(QWidget *parent, Project *logicProject) :
    QDialog(parent),
    ui(new Ui::LogSelectDialog)
{
    ui->setupUi(this);

    project = logicProject;

    initSettings();
}

int LogSelectDialog::selectedLogId(QWidget *parent, Project *logicProject)
{
    // создать объект самого себя
    LogSelectDialog *dlg = new LogSelectDialog(parent, logicProject);
    dlg->setModal(true);

    int result = -1;

    // в случае подтверждения выбора элемента в диалоге
    if(dlg->exec() == QDialog::Accepted)
    {
        // получить id выбранного журнала
        result = dlg->ui->logSelectTableWidget->selectedItems()[0]->text().toInt();
    }

    // удалить самого себя
    delete dlg;

    // и вернуть id журнала
    return result;
}

LogSelectDialog::~LogSelectDialog()
{
    delete ui;
}

void LogSelectDialog::initSettings()
{
    // настройка шапки таблицы
    ui->logSelectTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    // количество строк таблицы
    int rowCount = project->logsCount();
    ui->logSelectTableWidget->setRowCount(rowCount);

    // заполение ячеек таблицы
    for(int i = 0; i < rowCount; i++)
    {
        ui->logSelectTableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        ui->logSelectTableWidget->item(i, 0)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        ui->logSelectTableWidget->setItem(i, 1, new QTableWidgetItem(project->logName(i)));
        ui->logSelectTableWidget->item(i, 1)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }

    // выделение первой строки таблицы
    ui->logSelectTableWidget->selectRow(0);
}
