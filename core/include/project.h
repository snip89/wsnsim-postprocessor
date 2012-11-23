/*
 * project.h
 *
 *  Created on: May 8, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#ifndef PROJECT_H_
#define PROJECT_H_

#include <QObject>
#include <QString>
#include <QDir>
#include <QApplication>
#include <QLibrary>
#include <QStringList>
#include <QDebug>
#include <QErrorMessage>
#include <QtXml>

#include "iproject.h"
#include "simpleeventinfo.h"
#include "logdatatypes.h"
#include "projectParams.h"
#include "projectvalidator.h"
// TODO include projectParams herex

class Project : public IProject
{
public:
    friend class FiltrationWidget;
    friend class FormatAcceptingDialog;
    friend class ProjectValidator;

    Project(QString fileName);
    void load(QString &errorString);
    void save(QString &errorString);

    ProjectParams projectParams;

    /*virtual*/ SimpleEventInfo *info(int &size);

    int logsCount();
    QString logName(int id);
    QString projectName();
    QStringList paramsNames();
    LogDataType paramType(QString paramName);

    virtual ~Project();
private:
    QString projectFileName;
    int eventsInfoSize;
    SimpleEventInfo *eventsInfo;
    bool isLoaded;

    void loadEventsInfo();
    int loadEventsInfo(int iterator, QList<EventParams> events);
    int sizeOf(LogDataType type);
};

#endif /* PROJECT_H_ */
