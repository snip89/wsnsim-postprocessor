#include "mainsettings.h"
#include "ui_mainsettings.h"

MainSettings::MainSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainSettings)
{
    ui->setupUi(this);

    settingsFrameWidget = new QStackedWidget(this);
    ui->settingsFrameLayout->addWidget(settingsFrameWidget);

    createItems();

//    initSettingsTree();

    // addChildSettings(settings, 0, "");

    // connect(ui->settingsTree, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(activatedItem(QTreeWidgetItem*, int)));
    // connect(ui->settingsTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(activatedItem(QTreeWidgetItem*, int)));
    // connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

MainSettings::~MainSettings()
{
    deleteItems();

    delete settingsFrameWidget;
    delete ui;
}

void MainSettings::createItems()
{
    generalItem = new QTreeWidgetItem(ui->settingsTree);
    generalItem->setText(0, "General");

    generalCoreItem = new QTreeWidgetItem(generalItem);
    generalGuiItem = new QTreeWidgetItem(generalItem);

    hexVisualizationItem = new QTreeWidgetItem(ui->settingsTree);
    hexVisualizationColorsAndFontsItem = new QTreeWidgetItem(hexVisualizationItem);

    textVisualizationItem = new QTreeWidgetItem(ui->settingsTree);
    textVisualizationColorsAndFontsItem = new QTreeWidgetItem(textVisualizationItem);
}

void MainSettings::deleteItems()
{
    delete generalItem;
    delete generalCoreItem;
    delete generalGuiItem;

    delete hexVisualizationItem;
    delete hexVisualizationColorsAndFontsItem;

    delete textVisualizationItem;
    delete textVisualizationColorsAndFontsItem;
}

/*void MainSettings::addChildSettings(QSettings &settings, QTreeWidgetItem *parent, QString group)
{
    QTreeWidgetItem *item;

    settings.beginGroup(group);

    foreach(QString group, settings.childGroups())
    {
        if(group != tr("Defaults") && group != tr("Hidden") && group != "SYSTEM")
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
    if(button->text() == tr("OK"))
        emit settingsApplied();

    else if(button->text() == tr("Cancel"))
        emit settingsCanceled();
}
*/