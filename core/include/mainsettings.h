/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef MAINSETTINGS_H
#define MAINSETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QDebug>
#include <QStringList>
#include <QTreeWidgetItem>
#include <QString>
#include <QList>
#include <QStackedWidget>
#include <QAbstractButton>

namespace Ui {
    class MainSettings;
}

class MainSettings : public QWidget
{
    Q_OBJECT

public:
    QStackedWidget *settingsFrameWidget;

    MainSettings(QWidget *parent = 0);
    ~MainSettings();

private:
    Ui::MainSettings *ui;

    QList<QWidget*> widgets;

    void initSettingsTree();

    bool recursiveInitTree(QTreeWidgetItem *parentItem, QString group, QSettings &settings);

signals:
    void settingSelected(QString topLevelName, QString settingName);
    void settingsApplied();
    void settingsCanceled();

private slots:
    void activatedItem(QTreeWidgetItem *item, int column);
    void buttonClicked(QAbstractButton *button);
};

#endif // MAINSETTINGS_H
