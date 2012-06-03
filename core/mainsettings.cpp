#include "mainsettings.h"
#include "ui_mainsettings.h"

MainSettings::MainSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainSettings)
{
    ui->setupUi(this);

    settingsFrameWidget = new QStackedWidget(this);
    ui->settingsFrameLayout->addWidget(settingsFrameWidget);

//    initSettingsTree();

    addChildSettings(settings, 0, "");

    connect(ui->settingsTree, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(activatedItem(QTreeWidgetItem*, int)));
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

MainSettings::~MainSettings()
{
    delete settingsFrameWidget;
    delete ui;
}

void MainSettings::initSettingsTree()
{
    QSettings settings;

    qDebug() << settings.allKeys();

    QStringList topLevelGroups = settings.childGroups();

    if(topLevelGroups.size() != 0)
    {
        foreach(QString group, topLevelGroups)
        {
            if(group != "Defaults")
            {
                QTreeWidgetItem *item = new QTreeWidgetItem(QStringList(group), QTreeWidgetItem::Type);

                while(recursiveInitTree(item, group, settings));

                ui->settingsTree->addTopLevelItem(item);
            }
        }
    }
}

void MainSettings::addChildSettings(QSettings &settings, QTreeWidgetItem *parent, QString group)
{
    QTreeWidgetItem *item;

    settings.beginGroup(group);

    foreach(QString group, settings.childGroups())
    {
        if(group != "Defaults")
        {
            
            if(parent)
            {
                item = new QTreeWidgetItem(parent);
            }
            else
            {
                item = new QTreeWidgetItem(ui->settingsTree);
            }

            item->setText(0, group);
            addChildSettings(settings, item, group);

        }
    }

    settings.endGroup();
}

bool MainSettings::recursiveInitTree(QTreeWidgetItem *parentItem, QString group, QSettings &settings)
{
    settings.beginGroup(group);
    QStringList groups = settings.childGroups();

    if(groups.size() != 0)
    {
        foreach(QString group, groups)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(QStringList(group), QTreeWidgetItem::Type);

            while(recursiveInitTree(item, group, settings));

            parentItem->addChild(item);
        }
    }
    else
    {
        qDebug() << "1" << settings.group();
        settings.endGroup();
        return false;
    }

    qDebug() << "2" << settings.group();
    return true;
}

void MainSettings::activatedItem(QTreeWidgetItem *item, int column)
{
    QSettings settings;
    QStringList groups = settings.childGroups();

    QTreeWidgetItem *currentItem = item;
    QTreeWidgetItem *parentItem;

    QString itemText = item->text(column);

    if(groups.contains(itemText))
    {
        emit settingSelected(itemText, itemText);
        return;
    }

    while(!groups.contains(itemText))
    {
        parentItem = currentItem->parent();
        itemText = parentItem->text(column);
        currentItem = parentItem;
    }

    emit settingSelected(parentItem->text(column), item->text(column));
}

void MainSettings::buttonClicked(QAbstractButton *button)
{
    if(button->text() == "OK")
        emit settingsApplied();

    else if(button->text() == "Cancel")
        emit settingsCanceled();
}
