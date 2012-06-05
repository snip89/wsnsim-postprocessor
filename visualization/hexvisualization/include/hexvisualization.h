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
#include "ivisualizationsettings.h"
#include "abstracttextvisualization.h"
#include "records.h"
#include "staticrecordsreader.h"
#include "hexappearancecolorsandfontssettings.h"

class HexVisualization : public IVisualization, protected AbstractTextVisualization
{
public:
    explicit HexVisualization(QWidget *parent = 0);

    // interface methods

    /*virtual*/ IVisualizationSettings *visualizationSettings(QString name);

    /*virtual*/ void activity(bool status);
    /*virtual*/ void update(IProject *project, ILog *log);
    /*virtual*/ QWidget *getWidget();

    virtual ~HexVisualization();

private:
    HexAppearanceColorsAndFontsSettings *colorsAndFontsSettings;

    QSettings settings;

    void setSettings(QSettings &someSettings);

    /*virtual*/ void updatePage();
};

#endif // HEXVISUALIZATION_H
