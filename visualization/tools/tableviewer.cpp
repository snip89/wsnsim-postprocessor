#include "tableviewer.h"

TableViewer::TableViewer(QWidget *parent)
    : QTableWidget(parent)
{   
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

TableViewer::~TableViewer()
{
}
