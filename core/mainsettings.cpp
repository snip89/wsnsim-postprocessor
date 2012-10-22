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

    hexGuiSettings = new HexGuiSettings();
    hexGuiSettings->showCurrentSettings();

    textGuiSettings = new TextGuiSettings();
    textGuiSettings->showCurrentSettings();

    tableGuiSettings = new TableGuiSettings();
    tableGuiSettings->showCurrentSettings();

    createClientsSettings();

    initConnections();
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
    delete hexGuiSettings;
    delete textGuiSettings;
    delete tableGuiSettings;

    deleteClientsSettings();

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
    hexVisualizationGuiItem = new QTreeWidgetItem(hexVisualizationItem);

    hexVisualizationItem->setText(0, tr("Hex visualization"));
    hexVisualizationColorsAndFontsItem->setText(0, tr("Colors and Fonts"));
    hexVisualizationGuiItem->setText(0, tr("Gui"));

    textVisualizationItem = new QTreeWidgetItem(ui->settingsTree);
    textVisualizationColorsAndFontsItem = new QTreeWidgetItem(textVisualizationItem);
    textVisualizationGuiItem = new QTreeWidgetItem(textVisualizationItem);

    textVisualizationItem->setText(0, tr("Text visualization"));
    textVisualizationColorsAndFontsItem->setText(0, tr("Colors and Fonts"));
    textVisualizationGuiItem->setText(0, tr("Gui"));

    tableVisualizationItem = new QTreeWidgetItem(ui->settingsTree);
    tableVisualizationGuiItem = new QTreeWidgetItem(tableVisualizationItem);

    tableVisualizationItem->setText(0, tr("Table visualization"));
    tableVisualizationGuiItem->setText(0, tr("Gui"));

    localizationItem = new QTreeWidgetItem(ui->settingsTree);
    languageItem = new QTreeWidgetItem(localizationItem);

    localizationItem->setText(0, tr("Localization"));
    languageItem->setText(0, tr("Language"));

    realTimeItem = new QTreeWidgetItem(ui->settingsTree);
    realTimeItem->setText(0, tr("Real time"));

    clientsItem = new QTreeWidgetItem(realTimeItem);
    clientsItem->setText(0, tr("Clients"));

    createClientsItems();
}

void MainSettings::createClientsItems()
{
    IRealTimeSettings *rtSettings = StaticCoreUtils::getRealTimeSettings();

    foreach(QString client, rtSettings->clients())
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(clientsItem);
        item->setText(0, client);
        clientsItems.append(item);
    }
}

void MainSettings::deleteClientsItems()
{
    foreach(QTreeWidgetItem *item, clientsItems)
    {
        delete item;
    }
}

void MainSettings::createClientsSettings()
{
    IRealTimeSettings *rtSettings = StaticCoreUtils::getRealTimeSettings();

    foreach(QString client, rtSettings->clients())
    {
        ClientSettings *clientSettings = new ClientSettings();
        clientSettings->showCurrentSettings();
        clientSettings->setSettingsName(client);
        clientsSettings.append(clientSettings);
    }
}

void MainSettings::deleteClientsSettings()
{
    foreach(ClientSettings *rtSettings, clientsSettings)
    {
        delete rtSettings;
    }
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
    delete hexVisualizationGuiItem;
    delete hexVisualizationItem;

    delete textVisualizationColorsAndFontsItem;
    delete textVisualizationGuiItem;
    delete textVisualizationItem;

    delete tableVisualizationGuiItem;
    delete tableVisualizationItem;

    delete languageItem;
    delete localizationItem;

    deleteClientsItems();

    delete clientsItem;
    delete realTimeItem;
}

void MainSettings::showEmptySettings(QString name)
{
    emptySettings->setSettingsName(name);
    if(!settingsFrameWidget->isAncestorOf(emptySettings))
        settingsFrameWidget->addWidget(emptySettings);

    settingsFrameWidget->setCurrentWidget(emptySettings);
}

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

    else if(item == hexVisualizationGuiItem)
    {
        hexGuiSettings->setSettingsName(tr("Gui"));

        if(!settingsFrameWidget->isAncestorOf(hexGuiSettings))
            settingsFrameWidget->addWidget(hexGuiSettings);

        settingsFrameWidget->setCurrentWidget(hexGuiSettings);
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

    else if(item == textVisualizationGuiItem)
    {
        textGuiSettings->setSettingsName(tr("Gui"));

        if(!settingsFrameWidget->isAncestorOf(textGuiSettings))
            settingsFrameWidget->addWidget(textGuiSettings);

        settingsFrameWidget->setCurrentWidget(textGuiSettings);
    }

    else if(item == tableVisualizationItem)
    {
        showEmptySettings(tr("Table visualization"));
    }

    else if(item == tableVisualizationGuiItem)
    {
        tableGuiSettings->setSettingsName(tr("Gui"));

        if(!settingsFrameWidget->isAncestorOf(tableGuiSettings))
            settingsFrameWidget->addWidget(tableGuiSettings);

        settingsFrameWidget->setCurrentWidget(tableGuiSettings);
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

    else if(item == realTimeItem)
    {
        showEmptySettings(tr("Real time"));
    }

    else if(item == clientsItem)
    {
        showEmptySettings(tr("Clients"));
    }

    else
    {
        IRealTimeSettings *rtSettings = StaticCoreUtils::getRealTimeSettings();

        foreach(QTreeWidgetItem *currentItem, clientsItems)
        {
            if(item == currentItem)
            {
                ClientSettings *currentClientSettings;

                foreach(ClientSettings *clientSettings, clientsSettings)
                {
                    if(currentItem->text(0) == clientSettings->client())
                        currentClientSettings = clientSettings;
                }

                if(currentClientSettings != NULL)
                {
                    if(!settingsFrameWidget->isAncestorOf(currentClientSettings))
                        settingsFrameWidget->addWidget(currentClientSettings);

                    settingsFrameWidget->setCurrentWidget(currentClientSettings);
                }
            }
        }
    }
}

void MainSettings::dialogIsAccepted()
{
    localizationSettings->applySettings();
    generalGuiSettings->applySettings();
    generalCoreSettings->applySettings();
    hexColorsAndFontsSettings->applySettings();
    hexGuiSettings->applySettings();
    textColorsAndFontsSettings->applySettings();
    textGuiSettings->applySettings();
    tableGuiSettings->applySettings();

    emit settingsApplied();

    settings.setValue("Hidden/Gui/Settings_dialog_pos", pos());
}

void MainSettings::dialogIsRejected()
{
    localizationSettings->showCurrentSettings();
    generalGuiSettings->showCurrentSettings();
    generalCoreSettings->showCurrentSettings();
    hexColorsAndFontsSettings->showCurrentSettings();
    hexGuiSettings->showCurrentSettings();
    textColorsAndFontsSettings->showCurrentSettings();
    textGuiSettings->showCurrentSettings();
    tableGuiSettings->showCurrentSettings();

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
