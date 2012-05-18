#include "mainsettings.h"

MainSettings::MainSettings(QWidget *parent)
: QWidget(parent)
{
    ui.setupUi(this);

    settingsFrameWidget = new QStackedWidget(this);
    ui.settingsFrameLayout->addWidget(settingsFrameWidget);

    initSettingsTree();

    connect(ui.settingsTree, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(activatedItem(QTreeWidgetItem*, int)));
    connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

MainSettings::~MainSettings()
{
    delete settingsFrameWidget;
}

void MainSettings::initSettingsTree()
{
    QSettings settings;

    QStringList topLevelGroups = settings.childGroups();

    if(topLevelGroups.size() != 0)
    {
        foreach(QString group, topLevelGroups)
        {
            if(group != "Defaults")
            {
                QTreeWidgetItem *item = new QTreeWidgetItem(QStringList(group), QTreeWidgetItem::Type);

                while(recursiveInitTree(item, group, settings));

                ui.settingsTree->addTopLevelItem(item);
            }
        }
    }
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
        return false;

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
