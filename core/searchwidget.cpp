#include "searchwidget.h"
#include "ui_searchwidget.h"

SearchWidget::SearchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchWidget)
{
    ui->setupUi(this);

    connect(ui->searchNextPushButton, SIGNAL(clicked()), this, SLOT(findNextClicked()));
    connect(ui->searchPreviousPushButton, SIGNAL(clicked()), this, SLOT(findPreviousClicked()));
}

SearchWidget::~SearchWidget()
{
    delete ui;
}

void SearchWidget::findNextClicked()
{
    emit findNext(ui->searchLineEdit->text());
}

void SearchWidget::findPreviousClicked()
{
    emit findPrevious(ui->searchLineEdit->text());
}
