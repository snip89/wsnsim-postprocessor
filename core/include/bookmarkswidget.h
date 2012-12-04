#ifndef BOOKMARKSWIDGET_H
#define BOOKMARKSWIDGET_H

#include <QWidget>

#include "projectParams.h"
#include "ui_bookmarkswidget.h"

namespace Ui {
    class BookmarksWidget;
}

class BookmarksWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BookmarksWidget(QWidget *parent = 0);
    void update(VisualizationInfo info);
    void clearComboBox();
    virtual ~BookmarksWidget();
private:
    Ui::BookmarksWidget *ui;
private slots:
    void addBookmarkClicking();
    void removeBookmarkClicking();
    void applyBookmarkClicking();
signals:
    void addBookmarkClicked();
    void removeBookmarkClicked(QString bookmark);
    void applyBookmarkClicked(QString bookmark);
};

#endif // BOOKMARKSWIDGET_H
