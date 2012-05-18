#ifndef FILTRATIONWIDGET_H
#define FILTRATIONWIDGET_H

#include <QtGui/QWidget>
#include <QStringList>
#include <QList>
#include <QErrorMessage>
#include <QPoint>
#include <QMenu>
#include <QAction>
#include <QListWidgetItem>

#include "ui_filtrationwidget.h"
#include "log.h"
#include "project.h"
#include "abstractfilter.h"
#include "filter.h"
#include "staticlogfilter.h"
#include "booleanoperators.h"

class FiltrationWidget : public QWidget
{
    Q_OBJECT

public:
    FiltrationWidget(QWidget *parent = 0);

    void setCurrentProject(Project *project);
    void setCurrentLog(Log *log);
    void activate();
    void deactivate();

    ~FiltrationWidget();

private:
    Ui::FiltrationWidgetClass ui;

    Project *currentProject;
    Log *currentLog;

    QErrorMessage errorMessager;

    QList<AbstractFilter*> filters;

    void addBooleanOperators();
    void execute();

signals:
    void logFiltered(Log *newLog);
    void filtrationCanceled();

private slots:
    void addFilter();
    void buttonClicked(QAbstractButton *button);
    void showFiltrationListWidgetContextMenu(const QPoint& pos);
};

#endif // FILTRATIONWIDGET_H
