#include "localizationsettings.h"
#include "ui_localizationsettings.h"

LocalizationSettings::LocalizationSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LocalizationSettings)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

void LocalizationSettings::setSettingsName(QString name)
{
    ui->settingsNameLabel->setText(name);
}

void LocalizationSettings::showCurrentSettings()
{
    if(settings.value(tr("Localization/Language")).value<QString>() == tr("En"))
        ui->GUILanguageComboBox->setCurrentIndex(0);

    else if(settings.value(tr("Localization/Language")).value<QString>() == tr("Ru"))
        ui->GUILanguageComboBox->setCurrentIndex(1);
}

void LocalizationSettings::applySettings()
{
    if(ui->GUILanguageComboBox->currentIndex() == 0)
    {
        settings.setValue(tr("Localization/Language"), tr("En"));
    }

    else if(ui->GUILanguageComboBox->currentIndex() == 1)
    {
        settings.setValue(tr("Localization/Language"), tr("Ru"));
    }
}

LocalizationSettings::~LocalizationSettings()
{
    delete ui;
}

void LocalizationSettings::showDefaultSettings()
{
    if(settings.value("Defaults/Localization/Language").value<QString>() == "En")
    {
        ui->GUILanguageComboBox->setCurrentIndex(0);
    }

    else if(settings.value("Defaults/Localization/Language").value<QString>() == "Ru")
    {
        ui->GUILanguageComboBox->setCurrentIndex(1);
    }
}

void LocalizationSettings::buttonClicked(QAbstractButton *button)
{
    if(button->text() == tr("Apply"))
    {
        applySettings();
    }

    else if(button->text() == tr("Restore Defaults"))
        showDefaultSettings();
}
