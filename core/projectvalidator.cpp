#include "projectvalidator.h"

void ProjectValidator::validate(ProjectParams projectParams, QString &errorString)
{
    if(projectParams.events.systemEvents.size() == 0 &&
            projectParams.events.userEvents.size() == 0)
    {
        errorString = QObject::tr("No events in project");
        return;
    }

    int eventsID = 0;
    int argumentsID;

    foreach(EventParams event, projectParams.events.systemEvents)
    {
        if(event.eventInfo["ID"] == "")
        {
            errorString = QObject::tr("No ID in some event");
            return;
        }

        if(event.eventInfo["name"] == "")
        {
            errorString = QObject::tr("No name in some event");
            return;
        }

        if(eventsID == 0)
        {
            if(event.eventInfo["ID"].toInt() != 0)
            {
                errorString = QObject::tr("Bad event ID enumeration");
                return;
            }
        }

        if(eventsID != event.eventInfo["ID"].toInt())
        {
            errorString = QObject::tr("Bad event ID enumeration");
            return;
        }

        eventsID ++;

        argumentsID = 0;

        foreach(EventArgument argument, event.arguments)
        {
            if(argument["type"] == "")
            {
                errorString = QObject::tr("No type in some event argument");
                return;
            }

            if(argument["name"] == "")
            {
                errorString = QObject::tr("No name in some event argument");
                return;
            }

            if(argument["ID"] == "")
            {
                errorString = QObject::tr("No ID in some event argument");
                return;
            }

            if(argumentsID == 0)
            {
                if(argument["ID"].toInt() != 0)
                {
                    errorString = QObject::tr("Bad argument ID enumeration");
                    return;
                }
            }

            if(argumentsID != argument["ID"].toInt())
            {
                errorString = QObject::tr("Bad argument ID enumeration");
                return;
            }

            argumentsID ++;
        }
    }

    foreach(EventParams event, projectParams.events.userEvents)
    {
        if(event.eventInfo["ID"] == "")
        {
            errorString = QObject::tr("No ID in some event");
            return;
        }

        if(event.eventInfo["name"] == "")
        {
            errorString = QObject::tr("No name in some event");
            return;
        }

        if(eventsID != event.eventInfo["ID"].toInt())
        {
            errorString = QObject::tr("Bad event ID enumeration");
            return;
        }

        eventsID ++;

        argumentsID = 0;

        foreach(EventArgument argument, event.arguments)
        {
            if(argument["type"] == "")
            {
                errorString = QObject::tr("No type in some event argument");
                return;
            }

            if(argument["name"] == "")
            {
                errorString = QObject::tr("No name in some event argument");
                return;
            }

            if(argument["ID"] == "")
            {
                errorString = QObject::tr("No ID in some event argument");
                return;
            }

            if(argumentsID == 0)
            {
                if(argument["ID"].toInt() != 0)
                {
                    errorString = QObject::tr("Bad argument ID enumeration");
                    return;
                }
            }

            if(argumentsID != argument["ID"].toInt())
            {
                errorString = QObject::tr("Bad argument ID enumeration");
                return;
            }

            argumentsID ++;
        }
    }
}
