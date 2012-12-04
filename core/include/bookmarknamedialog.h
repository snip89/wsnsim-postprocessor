#ifndef BOOKMARKNAMEDIALOG_H
#define BOOKMARKNAMEDIALOG_H

#include <QDialog>
#include <QString>

#include "ui_bookmarknamedialog.h"

namespace Ui {
    class BookmarkNameDialog;
}

class BookmarkNameDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BookmarkNameDialog(QWidget *parent = 0);
    QString getName();
    virtual ~BookmarkNameDialog();
private:
    Ui::BookmarkNameDialog *ui;
};

#endif // BOOKMARKNAMEDIALOG_H
