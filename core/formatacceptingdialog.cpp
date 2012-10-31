#include "formatacceptingdialog.h"

FormatAcceptingDialog::FormatAcceptingDialog(Project *project, QString type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormatAcceptingDialog)
{
    this->project = project;
    this->type = type;

    ui->setupUi(this);
    initComboBox();
}

AttrInfo FormatAcceptingDialog::getArgument()
{
    QString value = ui->acceptionComboBox->itemData(
                ui->acceptionComboBox->currentIndex()).value<QString>();

    QStringList values = value.split(";");

    AttrInfo info;
    info["eventID"] = values[0];
    info["argumentID"] = values[1];
    info["type"] = values[2];

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
        foreach(EventArgument argument, event.arguments)
        {
            if(type == argument["type"])
            {
                QString info = event.eventInfo["ID"] + ";" + argument["ID"] + ";" + argument["type"];

                ui->acceptionComboBox->addItem("event: " + event.eventInfo["name"] + " / "
                                               "argument: " + argument["name"],
                                               QVariant(info));
            }
        }
    }

    foreach(EventParams event, project->projectParams.events.userEvents)
    {
        foreach(EventArgument argument, event.arguments)
        {
            if(type == argument["type"])
            {
                QString info = event.eventInfo["ID"] + ";" + argument["ID"] + ";" + argument["type"];

                ui->acceptionComboBox->addItem("event: " + event.eventInfo["name"] + " / "
                                               "argument: " + argument["name"],
                                               QVariant(info));
            }
        }
    }
}
