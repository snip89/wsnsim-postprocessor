#ifndef STATICLUALOADER_H
#define STATICLUALOADER_H

#include <QString>
#include <QFile>
#include <QMap>
#include <QByteArray>
#include <QVariant>
#include <QPair>
#include <QList>

extern "C"
{
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

#include "logdatatypes.h"
#include "staticrecordsreader.h"

class StaticLuaLoader
{
public:
    static QList< QPair<QString, QVariant> > exec(QString fileName, QByteArray array, QString &errorString);
    static void init();
    static void loadFile(QString fileName, QString &errorString);
    static QVariant convertBytes(char *data, int size, LogDataType type);
private:
    static lua_State *m_lua;
    static QMap<QString, int> refs;
};

#endif // STATICLUALOADER_H
