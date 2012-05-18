#ifndef GENERALCORESETTINGSWIDGET_H
#define GENERALCORESETTINGSWIDGET_H

#include <QtGui/QWidget>
#include <QString>
#include <QSettings>
#include <QAbstractButton>
#include <QMessageBox>

#include "ui_generalcoresettingswidget.h"
#include "ostools.h"

class GeneralCoreSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    GeneralCoreSettingsWidget(QWidget *parent = 0);

    void setSettingsName(QString name);
    void showCurrentSettings();
    void applySettings();

    ~GeneralCoreSettingsWidget();

private:
    Ui::GeneralCoreSettingsWidgetClass ui;

    QSettings settings;

    void showDefaultSettings();

private slots:
    void buttonClicked(QAbstractButton *button);
};

#endif // GENERALCORESETTINGSWIDGET_H
