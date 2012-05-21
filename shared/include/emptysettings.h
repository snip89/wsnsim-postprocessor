/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef EMPTYSETTINGS_H
#define EMPTYSETTINGS_H

#include <QWidget>
#include <QString>

namespace Ui {
    class EmptySettings;
}

class EmptySettings : public QWidget
{
    Q_OBJECT

public:
    explicit EmptySettings(QWidget *parent = 0);

    void setSettingsName(QString name);

    ~EmptySettings();

private:
    Ui::EmptySettings *ui;
};

#endif // EMPTYSETTINGS_H
