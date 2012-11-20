#ifndef QUICKSEARCHWIDGET_H
#define QUICKSEARCHWIDGET_H

#include <QWidget>

namespace Ui {
    class QuickSearchWidget;
}

class QuickSearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QuickSearchWidget(QWidget *parent = 0);
    virtual ~QuickSearchWidget();
private:
    Ui::QuickSearchWidget *ui;

private slots:
    void findNextClicked();
    void findPreviousClicked();

signals:
    void findNext(QString);
    void findPrevious(QString);
};

#endif // QUICKSEARCHWIDGET_H
