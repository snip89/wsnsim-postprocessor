#include "columnsselectiondialog.h"

ColumnsSelectionDialog::ColumnsSelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColumnsSelectionDialog)
{
    ui->setupUi(this);

    initListWidget();

    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

ColumnsSelectionDialog::~ColumnsSelectionDialog()
{
    delete ui;
}

void ColumnsSelectionDialog::initListWidget()
{
    QStringList argumentsNames = settings.value("Hidden/Gui/Project/Columns_names").value<QStringList>();
    QStringList argumentsState = settings.value("Hidden/Gui/Project/Columns_state").value<QStringList>();

    for(int i = 0; i < argumentsNames.size(); i ++)
    {
        QListWidgetItem *item = new QListWidgetItem(argumentsNames[i]);

        if(argumentsState[i] == "true")
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);

        ui->listWidget->addItem(item);
    }
}

void ColumnsSelectionDialog::buttonClicked(QAbstractButton *button)
{
    if(ui->buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole)
    {
        QStringList argumentsNames = settings.value("Hidden/Gui/Project/Columns_names").value<QStringList>();
        QStringList argumentsState = settings.value("Hidden/Gui/Project/Columns_state").value<QStringList>();

        for(int i = 0; i < argumentsNames.size(); i ++)
        {
            if(ui->listWidget->item(i)->checkState() == Qt::Checked)
                argumentsState[i] = "true";
            else
                argumentsState[i] = "false";
        }

        settings.setValue("Hidden/Gui/Project/Columns_names", argumentsNames);
        settings.setValue("Hidden/Gui/Project/Columns_state", argumentsState);

        done(QDialog::Accepted);
    }

    else if(ui->buttonBox->buttonRole(button) == QDialogButtonBox::RejectRole)
    {
        done(QDialog::Rejected);
    }
}
