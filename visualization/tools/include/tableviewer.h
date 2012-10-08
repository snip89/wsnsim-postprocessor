#ifndef TABLEVIEWER_H
#define TABLEVIEWER_H

#include <QTableWidget>
#include <QDebug>

class TableViewer : public QTableWidget
{
    Q_OBJECT
public:
    explicit TableViewer(QWidget *parent = 0);
    virtual ~TableViewer();
};

#endif // TABLEVIEWER_H
