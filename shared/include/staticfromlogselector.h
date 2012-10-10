#ifndef STATICFROMLOGSELECTOR_H
#define STATICFROMLOGSELECTOR_H

#include <QString>

#include "log.h"
#include "iproject.h"
#include "staticlogwriter.h"

class StaticFromLogSelector
{
public:
    static ILog* selectFromLog(ILog* log, IProject *project, QString name, int eventID);
private:
    static void appendToBuffer(char *fromBuffer, quint64 fromBufferPos, char *toBuffer, quint64 &toBufferPos, quint64 size);
};

#endif // STATICFROMLOGSELECTOR_H
