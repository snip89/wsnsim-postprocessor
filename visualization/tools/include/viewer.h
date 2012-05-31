/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef VIEWER_H
#define VIEWER_H

#include <QTextEdit>
#include <QList>
#include <QColor>
#include <QObject>
#include <QKeyEvent>
#include <QTextCursor>
#include <QDebug>

class Viewer : public QTextEdit
{
    Q_OBJECT
public:
    explicit Viewer(QWidget *parent = 0);

    void setLineColor(QColor color);

    virtual ~Viewer();

private slots:
    void highlightCurrentLine();

private:
    QColor lineColor;
};

#endif // VIEWER_H