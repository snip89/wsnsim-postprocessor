/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef TEXTVISUALIZATION_H
#define TEXTVISUALIZATION_H

#include <QWidget>
#include <QString>
#include <QSettings>
#include <QFont>

#include "ivisualization.h"
// #include "ivisualizationsettings.h"
#include "abstracttextvisualization.h"
#include "records.h"
#include "staticrecordsreader.h"
// #include "textappearancecolorsandfontssettings.h"
#include "staticvisualizationtools.h"

class TextVisualization : public IVisualization, protected AbstractTextVisualization
{
public:
    explicit TextVisualization(QWidget *parent = 0);

    // interface methods

    // /*virtual*/ IVisualizationSettings *visualizationSettings(QString name);

    /*virtual*/ void activity(bool status);
    /*virtual*/ void update(IProject *project, ILog *log,  QList<Format*> formats);
    /*virtual*/ void update(QList<Format*> formats);
    /*virtual*/ void searchNext(QString str);
    /*virtual*/ void searchPrevious(QString str);
    /*virtual*/ QWidget *getWidget();
    /*virtual*/ void fromLine(qint64 line);
    /*virtual*/ void getTextDocument(qint64 fromRecord, qint64 toRecord, QTextDocument &document);

    virtual ~TextVisualization();

private:
    // TextAppearanceColorsAndFontsSettings *colorsAndFontsSettings;

    QSettings settings;

    void setSettings(QSettings &someSettings);

    void updatePage();
    void updatePage(int cursorMoving);
};

#endif // TEXTVISUALIZATION_H
