#include "quicksearchwidget.h"
#include "ui_quicksearchwidget.h"

QuickSearchWidget::QuickSearchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuickSearchWidget)
{
    ui->setupUi(this);

    connect(ui->searchNextPushButton, SIGNAL(clicked()), this, SLOT(findNextClicked()));
    connect(ui->searchPreviousPushButton, SIGNAL(clicked()), this, SLOT(findPreviousClicked()));
}

QuickSearchWidget::~QuickSearchWidget()
{
    delete ui;
}

void QuickSearchWidget::findNextClicked()
{
    emit findNext(ui->searchLineEdit->text());
}

void QuickSearchWidget::findPreviousClicked()
{
    emit findPrevious(ui->searchLineEdit->text());
}
