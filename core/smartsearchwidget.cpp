#include "smartsearchwidget.h"
#include "ui_filtrationwidget.h"
#include <QPushButton>
#include "staticlogfilter.h"


SmartSearchWidget::SmartSearchWidget(QWidget *parent):FiltrationWidget(parent)
{
    ui->buttonBox->removeButton(ui->buttonBox->button(QDialogButtonBox::Ok));
    find_next_ = ui->buttonBox->addButton(tr("&Find"), QDialogButtonBox::ActionRole);

    setWindowTitle(tr("Smart search"));
    ui->filtrationGroupBox->setTitle(tr("Smart search"));

    connect(find_next_, SIGNAL(clicked()), this, SLOT(on_find_next()));
}

void SmartSearchWidget::on_find_next()
{
    quint64 pos = settings.value("Hidden/Core/Current_pos").value<int>();

    if(StaticLogFilter::findRecord(logs->at(currentLogId).log, filters, pos))
    {
        settings.setValue("Hidden/Core/Current_pos", pos);
        emit recordFinded();
    }
}
