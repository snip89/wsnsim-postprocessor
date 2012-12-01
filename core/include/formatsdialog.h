#ifndef FORMATSDIALOG_H
#define FORMATSDIALOG_H

#include <QDialog>
#include <QList>
#include <QMenu>
#include <QObject>
#include <QSettings>
#include <QFileDialog>
#include <QLibrary>
#include <QErrorMessage>

#include "ui_formatsdialog.h"
#include "formatParams.h"
#include "formatvalidator.h"
#include "project.h"
#include "formatacceptingdialog.h"

namespace Ui {
    class FormatsDialog;
}

class FormatsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FormatsDialog(Project *project, QList<Format*> formats, QWidget *parent = 0);
    QList<Format*> getFormats();
    virtual ~FormatsDialog();
private:
    QList<Format*> formats;

    QSettings settings;

    Project *project;

    QErrorMessage errorMessager;

    Ui::FormatsDialog *ui;

    void loadFormat();

    void updateList();
private slots:
    void addButtonClicked();
    void removeButtonClicked();
    void removeAllButtonClicked();
};

#endif // FORMATSDIALOG_H
