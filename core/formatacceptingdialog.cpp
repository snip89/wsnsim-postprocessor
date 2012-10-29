#include "formatacceptingdialog.h"

FormatAcceptingDialog::FormatAcceptingDialog(Project *project, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormatAcceptingDialog)
{
    this->project = project;

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
            QString info = event.eventInfo["ID"] + ";" + argument["ID"] + ";" + argument["type"];

            ui->acceptionComboBox->addItem(argument["name"], QVariant(info));
        }
    }
}
