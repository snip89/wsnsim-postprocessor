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

#include "emptysettings.h"
#include "generalcoresettings.h"
#include "generalguisettings.h"

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
    QSettings settings;

    QTreeWidgetItem *generalItem;
    QTreeWidgetItem *generalCoreItem;
    QTreeWidgetItem *generalGuiItem;
    QTreeWidgetItem *hexVisualizationItem;
    QTreeWidgetItem *textVisualizationItem;
    QTreeWidgetItem *hexVisualizationColorsAndFontsItem;
    QTreeWidgetItem *textVisualizationColorsAndFontsItem;

    Ui::MainSettings *ui;

    void createItems();
    void deleteItems();

    // QList<QWidget*> widgets;

    // void addChildSettings(QSettings &settings, QTreeWidgetItem *parent, QString group);

signals:
    // void settingSelected(QString topLevelName, QString settingName);
    // void settingsApplied();
    // void settingsCanceled();

private slots:
    // void activatedItem(QTreeWidgetItem *item, int column);
    // void buttonClicked(QAbstractButton *button);
};

#endif // MAINSETTINGS_H
