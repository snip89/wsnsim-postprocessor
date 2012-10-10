#ifndef TABLEVIEWER_H
#define TABLEVIEWER_H

#include <QTableWidget>
#include <QDebug>
#include <QSize>

#define TVIEWER_PADDING_KOEFF 15

class TableViewer : public QTableWidget
{
    Q_OBJECT
public:
    explicit TableViewer(QWidget *parent = 0);

    int rowSize(int index);

    virtual ~TableViewer();
};

#endif // TABLEVIEWER_H
