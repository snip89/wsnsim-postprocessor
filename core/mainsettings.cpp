#include "mainsettings.h"
#include "ui_mainsettings.h"

MainSettings::MainSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainSettings)
{
    ui->setupUi(this);

    settingsFrameWidget = new QStackedWidget(this);
    ui->settingsFrameLayout->addWidget(settingsFrameWidget);

    createItems();

    emptySettings = new EmptySettings();

    localizationSettings = new LocalizationSettings();
    localizationSettings->showCurrentSettings();

    generalGuiSettings = new GeneralGuiSettings();
    generalGuiSettings->showCurrentSettings();

    generalCoreSettings = new GeneralCoreSettings();
    generalCoreSettings->showCurrentSettings();

    hexColorsAndFontsSettings = new HexAppearanceColorsAndFontsSettings();
    hexColorsAndFontsSettings->showCurrentSettings();

    textColorsAndFontsSettings = new TextAppearanceColorsAndFontsSettings();
    textColorsAndFontsSettings->showCurrentSettings();

    initConnections();

//    initSettingsTree();

    // addChildSettings(settings, 0, "");

    // connect(ui->settingsTree, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(activatedItem(QTreeWidgetItem*, int)));
    // connect(ui->settingsTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(activatedItem(QTreeWidgetItem*, int)));
    // connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

MainSettings::~MainSettings()
{
    deleteItems();

    delete emptySettings;
    delete localizationSettings;
    delete generalCoreSettings;
    delete generalGuiSettings;
    delete hexColorsAndFontsSettings;
    delete textColorsAndFontsSettings;

    delete settingsFrameWidget;
    delete ui;
}

void MainSettings::createItems()
{
    generalItem = new QTreeWidgetItem(ui->settingsTree);
    generalCoreItem = new QTreeWidgetItem(generalItem);
    generalGuiItem = new QTreeWidgetItem(generalItem);

    generalItem->setText(0, tr("General"));
    generalCoreItem->setText(0, tr("Core"));
    generalGuiItem->setText(0, tr("Gui"));

    hexVisualizationItem = new QTreeWidgetItem(ui->settingsTree);
    hexVisualizationColorsAndFontsItem = new QTreeWidgetItem(hexVisualizationItem);

    hexVisualizationItem->setText(0, tr("Hex visualization"));
    hexVisualizationColorsAndFontsItem->setText(0, tr("Colors and Fonts"));

    textVisualizationItem = new QTreeWidgetItem(ui->settingsTree);
    textVisualizationColorsAndFontsItem = new QTreeWidgetItem(textVisualizationItem);

    textVisualizationItem->setText(0, tr("Text visualization"));
    textVisualizationColorsAndFontsItem->setText(0, tr("Colors and Fonts"));

    localizationItem = new QTreeWidgetItem(ui->settingsTree);
    languageItem = new QTreeWidgetItem(localizationItem);

    localizationItem->setText(0, tr("Localization"));
    languageItem->setText(0, tr("Language"));
}

void MainSettings::initConnections()
{
    connect(ui->settingsTree, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(activatedItem(QTreeWidgetItem*, int)));
    connect(ui->settingsTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(activatedItem(QTreeWidgetItem*, int)));

    connect(this, SIGNAL(accepted()), this, SLOT(dialogIsAccepted()));
    connect(this, SIGNAL(rejected()), this, SLOT(dialogIsRejected()));

    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

void MainSettings::deleteItems()
{
    delete generalCoreItem;
    delete generalGuiItem;

    delete generalItem;

    delete hexVisualizationColorsAndFontsItem;
    delete hexVisualizationItem;

    delete textVisualizationColorsAndFontsItem;
    delete textVisualizationItem;

    delete languageItem;
    delete localizationItem;
}

void MainSettings::showEmptySettings(QString name)
{
    emptySettings->setSettingsName(name);
    if(!settingsFrameWidget->isAncestorOf(emptySettings))
        settingsFrameWidget->addWidget(emptySettings);

    settingsFrameWidget->setCurrentWidget(emptySettings);
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
}*/

void MainSettings::activatedItem(QTreeWidgetItem *item, int column)
{
    if(item == generalItem)
    {
        showEmptySettings(tr("General"));
    }

    else if(item == generalCoreItem)
    {
        generalCoreSettings->setSettingsName(tr("Core"));

        if(!settingsFrameWidget->isAncestorOf(generalCoreSettings))
            settingsFrameWidget->addWidget(generalCoreSettings);

        settingsFrameWidget->setCurrentWidget(generalCoreSettings);
    }

    else if(item == generalGuiItem)
    {
        generalGuiSettings->setSettingsName(tr("Gui"));

        if(!settingsFrameWidget->isAncestorOf(generalGuiSettings))
            settingsFrameWidget->addWidget(generalGuiSettings);

        settingsFrameWidget->setCurrentWidget(generalGuiSettings);
    }

    else if(item == hexVisualizationItem)
    {
        showEmptySettings(tr("Hex visualization"));
    }

    else if(item == hexVisualizationColorsAndFontsItem)
    {
        hexColorsAndFontsSettings->setSettingsName(tr("Colors and Fonts"));

        if(!settingsFrameWidget->isAncestorOf(hexColorsAndFontsSettings))
            settingsFrameWidget->addWidget(hexColorsAndFontsSettings);

        settingsFrameWidget->setCurrentWidget(hexColorsAndFontsSettings);
    }

    else if(item == textVisualizationItem)
    {
        showEmptySettings(tr("Text visualization"));
    }

    else if(item == textVisualizationColorsAndFontsItem)
    {
        textColorsAndFontsSettings->setSettingsName(tr("Colors and Fonts"));

        if(!settingsFrameWidget->isAncestorOf(textColorsAndFontsSettings))
            settingsFrameWidget->addWidget(textColorsAndFontsSettings);

        settingsFrameWidget->setCurrentWidget(textColorsAndFontsSettings);
    }

    else if(item == localizationItem)
    {
        showEmptySettings(tr("Localization"));
    }

    else if(item == languageItem)
    {
        localizationSettings->setSettingsName(tr("Language"));

        if(!settingsFrameWidget->isAncestorOf(localizationSettings))
            settingsFrameWidget->addWidget(localizationSettings);

        settingsFrameWidget->setCurrentWidget(localizationSettings);
    }
}

void MainSettings::dialogIsAccepted()
{
    localizationSettings->applySettings();
    generalGuiSettings->applySettings();
    generalCoreSettings->applySettings();
    hexColorsAndFontsSettings->applySettings();
    textColorsAndFontsSettings->applySettings();

    settings.setValue("Hidden/Gui/Settings_dialog_pos", pos());
}

void MainSettings::dialogIsRejected()
{
    localizationSettings->showCurrentSettings();
    generalGuiSettings->showCurrentSettings();
    generalCoreSettings->showCurrentSettings();
    hexColorsAndFontsSettings->showCurrentSettings();
    textColorsAndFontsSettings->showCurrentSettings();

    settings.setValue("Hidden/Gui/Settings_dialog_pos", pos());
}

void MainSettings::buttonClicked(QAbstractButton *button)
{
    if(ui->buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole)
    {
        done(QDialog::Accepted);
    }

    else if(ui->buttonBox->buttonRole(button) == QDialogButtonBox::RejectRole)
    {
        done(QDialog::Rejected);
    }
}
