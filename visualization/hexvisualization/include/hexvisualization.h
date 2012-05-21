/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef HEXVISUALIZATION_H
#define HEXVISUALIZATION_H

#include <QWidget>

#include "ivisualization.h"
#include "abstracttextvisualization.h"
#include "records.h"
#include "staticrecordsreader.h"

class HexVisualization : public IVisualization, protected AbstractTextVisualization
{
public:
    explicit HexVisualization(QWidget *parent = 0);

    // interface methods
    /*virtual*/ void activity(bool status);
    /*virtual*/ void update(IProject *project, ILog *log);
    /*virtual*/ QWidget *getWidget();

    virtual ~HexVisualization();

private:
    /*virtual*/ void updatePage();
};

#endif // HEXVISUALIZATION_H
