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
#include <QtXml>

#include "iproject.h"
#include "simpleeventinfo.h"
#include "logdatatypes.h"
#include "projectParams.h"
// TODO include projectParams herex

class Project : public IProject
{
public:
    friend class FiltrationWidget;
    friend class FormatAcceptingDialog;

    Project(QString fileName);
    void load(QString &errorString);

    /*virtual*/ SimpleEventInfo *info(int &size);

    int logsCount();
    QString logName(int id);
    QString projectName();
    QStringList paramsNames();
    LogDataType paramType(QString paramName);
    bool isInjectedColumnsSettings(QString &errorString);
    void injectColumnsSettings(QString settings, QString &errorString);
    void getInjectedColumnsSettings(QStringList &columnsNames, QStringList &columnsState, QString &errorString);
    virtual ~Project();
private:
    QString projectFileName;
    ProjectParams projectParams;
    int eventsInfoSize;
    SimpleEventInfo *eventsInfo;
    bool isLoaded;

    void loadEventsInfo();
    int loadEventsInfo(int iterator, QList<EventParams> events);
    int sizeOf(LogDataType type);
};

#endif /* PROJECT_H_ */
