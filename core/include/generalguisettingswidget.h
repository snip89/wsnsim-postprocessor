#ifndef GENERALGUISETTINGSWIDGET_H
#define GENERALGUISETTINGSWIDGET_H

#include <QtGui/QWidget>
#include <QString>
#include <QSettings>
#include <QAbstractButton>

#include "ui_generalguisettingswidget.h"

class GeneralGuiSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    GeneralGuiSettingsWidget(QWidget *parent = 0);

    void setSettingsName(QString name);
    void showCurrentSettings();
    void applySettings();

    ~GeneralGuiSettingsWidget();

private:
    Ui::GeneralGuiSettingsWidgetClass ui;

    QSettings settings;

    void showDefaultSettings();

private slots:
    void buttonClicked(QAbstractButton *button);
};

#endif // GENERALGUISETTINGSWIDGET_H
