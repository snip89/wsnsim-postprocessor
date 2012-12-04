#include "bookmarkswidget.h"

BookmarksWidget::BookmarksWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookmarksWidget)
{
    ui->setupUi(this);

    connect(ui->savePushButton, SIGNAL(clicked()), this, SLOT(addBookmarkClicking()));
    connect(ui->removePushButton, SIGNAL(clicked()), this, SLOT(removeBookmarkClicking()));
    connect(ui->applyPushButton, SIGNAL(clicked()), this, SLOT(applyBookmarkClicking()));
}

void BookmarksWidget::update(VisualizationInfo info)
{
    ui->comboBox->clear();

    QString bookmarksInfo = info.bookmarksSettings;
    QStringList bookmarks = bookmarksInfo.split(';');

    foreach(QString bookmark, bookmarks)
    {
        QStringList bookmarkArgs = bookmark.split(',');
        ui->comboBox->addItem(bookmarkArgs[0], QVariant(bookmark));
    }

    if(ui->comboBox->currentText() == "")
        ui->applyPushButton->setEnabled(false);
    else
        ui->applyPushButton->setEnabled(true);
}

void BookmarksWidget::clearComboBox()
{
    ui->comboBox->clear();
}

void BookmarksWidget::addBookmarkClicking()
{
    emit addBookmarkClicked();
}

void BookmarksWidget::removeBookmarkClicking()
{
    emit removeBookmarkClicked(ui->comboBox->itemData(ui->comboBox->currentIndex()).toString());
}

void BookmarksWidget::applyBookmarkClicking()
{
    emit applyBookmarkClicked(ui->comboBox->itemData(ui->comboBox->currentIndex()).toString());
}

BookmarksWidget::~BookmarksWidget()
{
    delete ui;
}
