#ifndef SMARTSEARCHWIDGET_H
#define SMARTSEARCHWIDGET_H

#include "filtrationwidget.h"
#include "ilog.h"
#include <QPushButton>

class QPushButton;

class SmartSearchWidget : public FiltrationWidget{
    Q_OBJECT
public:
    explicit SmartSearchWidget(QWidget *parent = 0);
private slots:
    void on_find_next();
private:
    QPushButton* find_next_, cancel_;
signals:
    void recordFinded();
};

#endif // SMARTSEARCHWIDGET_H
