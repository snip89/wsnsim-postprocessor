#ifndef TABLEVISUALIZATION_H
#define TABLEVISUALIZATION_H

#include <QSettings>
#include <QWidget>

#include "ivisualization.h"
#include "log.h"
#include "abstracttablevisualization.h"
#include "staticfromlogselector.h"

class TableVisualization : public IVisualization, protected AbstractTableVisualization
{
public:
    explicit TableVisualization(QWidget *parent = 0);

    // interface methods

    /*virtual*/ void activity(bool status);
    /*virtual*/ void update(IProject *project, ILog *log);
    /*virtual*/ void update();
    /*virtual*/ QWidget *getWidget();
    /*virtual*/ void fromLine(qint64 line);

    virtual ~TableVisualization();
private:
    QSettings settings;

    bool firstTime;

    void setSettings(QSettings &someSettings);

    void updatePage();
    void updatePage(int cursorMoving);

    void initEventSelector();
};

#endif // TABLEVISUALIZATION_H
