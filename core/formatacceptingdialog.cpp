#include "formatacceptingdialog.h"

FormatAcceptingDialog::FormatAcceptingDialog(Project *project, QString name, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormatAcceptingDialog)
{
    this->project = project;
    this->name = name;

    ui->setupUi(this);

    connect(ui->acceptionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateArgComboBox(int)));

    initComboBox();
}

AttrInfo FormatAcceptingDialog::getArgument()
{
    QString value = ui->acceptionComboBox->itemData(
                ui->acceptionComboBox->currentIndex()).value<QString>();

    QStringList values = value.split(";");

    AttrInfo info;
    info["eventID"] = values[0];
    //info["argumentID"] = values[1];
    info["eventType"] = values[1];

    QString argValue = ui->acceptionArgComboBox->itemData(
                ui->acceptionArgComboBox->currentIndex()).value<QString>();

    info["argumentID"] = argValue;

    return info;
}

FormatAcceptingDialog::~FormatAcceptingDialog()
{
    delete ui;
}

void FormatAcceptingDialog::initComboBox()
{
    foreach(EventParams event, project->projectParams.events.systemEvents)
    {
        //foreach(EventArgument argument, event.arguments)
        //{
            //if(name == argument["name"])
            //{
                QString info = event.eventInfo["ID"] + ";" + event.eventInfo["name"];

                ui->acceptionComboBox->addItem("event: " + event.eventInfo["name"],
                                               QVariant(info));
            //}
        //}
    }

    foreach(EventParams event, project->projectParams.events.userEvents)
    {
        //foreach(EventArgument argument, event.arguments)
        //{
            //if(name == argument["name"])
            //{
                QString info = event.eventInfo["ID"] + ";" + event.eventInfo["name"];

                ui->acceptionComboBox->addItem("event: " + event.eventInfo["name"],
                                               QVariant(info));
            //}
        //}
    }
}

void FormatAcceptingDialog::updateArgComboBox(int index)
{
    ui->acceptionArgComboBox->clear();

    EventParams currentEvent;

    foreach(EventParams event, project->projectParams.events.systemEvents)
    {
        if(event.eventInfo["ID"] == QString::number(index))
        {
            currentEvent = event;
        }
    }

    foreach(EventParams event, project->projectParams.events.userEvents)
    {
        if(event.eventInfo["ID"] == QString::number(index))
        {
            currentEvent = event;
        }
    }

    foreach(EventArgument argument, currentEvent.arguments)
    {
        if(argument["type"] == "ByteArray")
        {
            QString info = argument["ID"];
            ui->acceptionArgComboBox->addItem("argument: " + argument["name"], QVariant(info));
        }
    }
}
