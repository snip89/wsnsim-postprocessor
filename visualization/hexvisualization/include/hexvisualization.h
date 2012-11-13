/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef HEXVISUALIZATION_H
#define HEXVISUALIZATION_H

#include <QWidget>
#include <QString>
#include <QSettings>
#include <QFont>

#include "ivisualization.h"
#include "abstracttextvisualization.h"
#include "records.h"
#include "staticrecordsreader.h"

class HexVisualization : public IVisualization, protected AbstractTextVisualization
{
public:
    explicit HexVisualization(QWidget *parent = 0);

    // interface methods

    // /*virtual*/ IVisualizationSettings *visualizationSettings(QString name);

    /*virtual*/ void activity(bool status);
    /*virtual*/ void update(IProject *project, ILog *log,  QList<Format*> formats);
    /*virtual*/ void update(QList<Format*> formats);
    /*virtual*/ void searchNext(QString str);
    /*virtual*/ void searchPrevious(QString str);
    /*virtual*/ QWidget *getWidget();
    /*virtual*/ void fromLine(qint64 line);

    virtual ~HexVisualization();

private:
    // HexAppearanceColorsAndFontsSettings *colorsAndFontsSettings;

    QSettings settings;

    void setSettings(QSettings &someSettings);

    void updatePage();
    void updatePage(int cursorMoving);
};

#endif // HEXVISUALIZATION_H
