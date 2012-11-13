#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>

namespace Ui {
    class SearchWidget;
}

class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWidget(QWidget *parent = 0);
    virtual ~SearchWidget();
private:
    Ui::SearchWidget *ui;

private slots:
    void findNextClicked();
    void findPreviousClicked();

signals:
    void findNext(QString);
    void findPrevious(QString);
};

#endif // SEARCHWIDGET_H
