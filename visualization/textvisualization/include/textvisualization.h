/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef TEXTVISUALIZATION_H
#define TEXTVISUALIZATION_H

#include <QWidget>
#include <QString>

#include "ivisualization.h"
#include "ivisualizationsettings.h"
#include "abstracttextvisualization.h"
#include "records.h"
#include "staticrecordsreader.h"

class TextVisualization : public IVisualization, protected AbstractTextVisualization
{
public:
    explicit TextVisualization(QWidget *parent = 0);

    // interface methods

    /*virtual*/ IVisualizationSettings *visualizationSettings(QString name);

    /*virtual*/ void activity(bool status);
    /*virtual*/ void update(IProject *project, ILog *log);
    /*virtual*/ QWidget *getWidget();

    virtual ~TextVisualization();

private:
    /*virtual*/ void updatePage();
};

#endif // TEXTVISUALIZATION_H
