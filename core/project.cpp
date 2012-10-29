/*
 * project.cpp
 *
 *  Created on: May 8, 2012
 *      Author: Sergey Popov (snip89@mail.ru)
 */

#include "project.h"

Project::Project(QString fileName)
{
    projectFileName = fileName;
    isLoaded = false;
}

Project::~Project()
{
    if(isLoaded)
    {
        for(int i = 0; i < eventsInfoSize; i ++)
        {
            for(int j = 0; j < eventsInfo[i].argsCount; j ++)
                delete eventsInfo[i].argsInfo[j].name;

            delete[] eventsInfo[i].argsInfo;
            delete eventsInfo[i].type;
            delete eventsInfo[i].group;
        }

        delete[] eventsInfo;
    }
}

void Project::load(QString &errorString)
{
    errorString = QString::null;
    QDir::setCurrent(QApplication::applicationDirPath());
    QString libraryName;

    QLibrary projectDataLibrary("./projectData");
    if(!projectDataLibrary.load())
    {
        errorString = projectDataLibrary.errorString();
        return;
    }

    typedef ProjectParams(*projectDataLoad) (QString& projectFileName, QString* errorMessage);
    projectDataLoad load = (projectDataLoad) projectDataLibrary.resolve("load");

    projectParams = load(projectFileName, &errorString);

    foreach(LogFileInfo logInfo, projectParams.logFiles)
    {
        if(!logInfo.contains("name") || !logInfo.contains("ID"))
        {
            errorString = QObject::tr("Bad log files info");
            return;
        }
    }

    loadEventsInfo();

    for(int i = 0; i < eventsInfoSize; i ++)
    {
        qDebug() << "event level";
        qDebug() << "   event size:" + QString::number(eventsInfo[i].size);
        qDebug() << "   args count:" + QString::number(eventsInfo[i].argsCount);
        qDebug() << "   event type:" + *eventsInfo[i].type;
        qDebug() << "   event group:" + *eventsInfo[i].group;
        qDebug() << "   arguments level";

        for(int j = 0; j < eventsInfo[i].argsCount; j ++)
        {
            qDebug() << "      type:" + QString::number(eventsInfo[i].argsInfo[j].type);
            qDebug() << "      size:" + QString::number(eventsInfo[i].argsInfo[j].size);
            qDebug() << "      name:" + *eventsInfo[i].argsInfo[j].name;
        }
    }

    isLoaded = true;
}

SimpleEventInfo *Project::info(int &size)
{
    size = eventsInfoSize;
    return eventsInfo;
}

int Project::logsCount()
{
    return projectParams.logFiles.size();
}

QString Project::logName(int id)
{
    QString logName = "NONAME";

    foreach (LogFileInfo logFileInfo, projectParams.logFiles)
    {
        if(logFileInfo["ID"] == QString::number(id))
        {
            logName = logFileInfo["name"];
        }
    }

    return logName;
}

QString Project::projectName()
{
    return projectFileName;
}

QStringList Project::paramsNames()
{
    QStringList result;
    result.append("time");
    result.append("eventID");

    foreach(EventParams params, projectParams.events.systemEvents)
    {
        foreach(EventArgument argument, params.arguments)
        {
            if(!result.contains(argument["name"]))
                result.append(argument["name"]);
        }
    }

    foreach(EventParams params, projectParams.events.userEvents)
    {
        foreach(EventArgument argument, params.arguments)
        {
            if(!result.contains(argument["name"]))
                result.append(argument["name"]);
        }
    }

    return result;
}

LogDataType Project::paramType(QString paramName)
{
    LogDataType result = UNKNOWN_TYPE;

    if(paramName == "time")
        return UINT64_TYPE;

    if(paramName == "eventID")
        return UINT8_TYPE;

    foreach(EventParams params, projectParams.events.systemEvents)
    {
        foreach(EventArgument argument, params.arguments)
        {
            if(argument["name"] == paramName)
            {
                result = LogDataTypes::toLogDataType(argument["type"]);
                return result;
            }
        }
    }

    foreach(EventParams params, projectParams.events.userEvents)
    {
        foreach(EventArgument argument, params.arguments)
        {
            if(argument["name"] == paramName)
            {
                result = LogDataTypes::toLogDataType(argument["type"]);
                return result;
            }
        }
    }

    return result;
}

void Project::loadEventsInfo()
{
    eventsInfoSize = projectParams.events.systemEvents.size();
    eventsInfoSize += projectParams.events.userEvents.size();

    eventsInfo = new SimpleEventInfo[eventsInfoSize];

    int iterator = loadEventsInfo(0, projectParams.events.systemEvents);
    loadEventsInfo(iterator, projectParams.events.userEvents);
}

int Project::loadEventsInfo(int iterator, QList<EventParams> events)
{
    for(int i = 0; i < events.size(); i++)
    {
        eventsInfo[iterator].type = new QString(events[i].eventInfo["name"]);
        eventsInfo[iterator].group = new QString(events[i].eventInfo["group"]);

        int argsCountTemp = events[i].arguments.size();
        eventsInfo[iterator].argsCount = argsCountTemp;

        eventsInfo[iterator].argsInfo = new SimpleArgInfo[argsCountTemp];

        int eventSize = 0;
        bool isDinamicArg = false;

        for(int j = 0; j < argsCountTemp; j++)
        {
            eventsInfo[iterator].argsInfo[j].name = new QString(events[i].arguments[j]["name"]);

            QString stringArgType = events[i].arguments[j]["type"];

            LogDataType newArgType = LogDataTypes::toLogDataType(stringArgType);
            eventsInfo[iterator].argsInfo[j].type = newArgType;
            int argSize = sizeOf(newArgType);
            eventsInfo[iterator].argsInfo[j].size = argSize;

            if(newArgType == BYTE_ARRAY_TYPE || newArgType == STRING_TYPE)
                isDinamicArg = true;

            eventSize += argSize;
        }

        if(!isDinamicArg)
            eventsInfo[iterator].size = eventSize;
        else
            eventsInfo[iterator].size = -1;

        iterator ++;
    }

    return iterator;
}

int Project::sizeOf(LogDataType type)
{
    if(type == UINT8_TYPE)
        return sizeof(quint8);

    else if(type == UINT16_TYPE)
        return sizeof(quint16);

    else if(type == UINT32_TYPE)
        return sizeof(quint32);

    else if(type == UINT64_TYPE)
        return sizeof(quint64);

    else if(type == INT32_TYPE)
        return sizeof(qint32);

    else if(type == DOUBLE_TYPE)
        return sizeof(double);

    else if(type == BYTE_ARRAY_TYPE)
        return 1;

    else if(type == STRING_TYPE)
        return 2;

    return -1;
}
