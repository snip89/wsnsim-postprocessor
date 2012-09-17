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
#include <QSettings>
#include <QVariant>
#include <QString>
#include <QDebug>

#define VIEWER_PADDING_KOEFF 10

class Viewer : public QTextEdit
{
    Q_OBJECT
public:
    explicit Viewer(QString group, QWidget *parent = 0);

    virtual ~Viewer();

private slots:
    void highlightCurrentLine();

private:
    QColor lineColor;
};

#endif // VIEWER_H
