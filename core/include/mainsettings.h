#ifndef MAINSETTINGS_H
#define MAINSETTINGS_H

#include <QtGui/QWidget>
#include <QSettings>
#include <QDebug>
#include <QStringList>
#include <QTreeWidgetItem>
#include <QString>
#include <QList>
#include <QWidget>
#include <QStackedWidget>
#include <QAbstractButton>

#include "ui_mainsettings.h"

class MainSettings : public QWidget
{
    Q_OBJECT

public:
    QStackedWidget *settingsFrameWidget;

    MainSettings(QWidget *parent = 0);
    ~MainSettings();

private:
    Ui::MainSettingsClass ui;

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
