#include "staticlualoader.h"

lua_State* StaticLuaLoader::m_lua = 0;
QMap<QString, int> StaticLuaLoader::refs;

QList< QPair<QString, QVariant> > StaticLuaLoader::exec(QString fileName, QByteArray array, QString &errorString)
{
    QList< QPair<QString, QVariant> > result;

    int ref = refs[fileName];

    lua_rawgeti(m_lua, LUA_REGISTRYINDEX, ref);

    lua_createtable(m_lua, array.size(), 0);
    for (quint8 i = 0; i < array.size();  i++) {
        lua_pushnumber(m_lua, array[i]);
        lua_rawseti(m_lua, -2, i + 1);
    }

    int ret = lua_pcall(m_lua, 1, 1, 0);
    if (ret) {
        errorString = QObject::tr("Can't execute lua function format");
        lua_pop(m_lua, 1);
        return result;
    }

    if (lua_istable(m_lua, -1))
    {
        lua_pushnil(m_lua);

        while(lua_next(m_lua, -2))
        {
            QString name;
            LogDataType convertedType;

            if(lua_istable(m_lua, -1))
            {
                lua_getfield(m_lua, -1, "error");
                if(lua_isstring(m_lua, -1))
                {
                    QString error = lua_tostring(m_lua, -1);

                    lua_pop(m_lua, 3);
                    errorString = error;
                    return result;
                }

                lua_pop(m_lua, 1);

                lua_getfield(m_lua, -1, "name");
                if(lua_isstring(m_lua, -1))
                {
                    name = lua_tostring(m_lua, -1);
                }

                lua_pop(m_lua, 1);

                lua_getfield(m_lua, -1, "type");
                if(lua_isstring(m_lua, -1))
                {
                    QString type = lua_tostring(m_lua, -1);
                    convertedType = LogDataTypes::toLogDataType(type);
                }

                lua_pop(m_lua, 1);

                lua_getfield(m_lua, -1, "value");
                if(lua_istable(m_lua, -1))
                {
                    quint8 size = luaL_getn(m_lua, -1);
                    char* data = new char[size];
                    for (quint8 idx = 1; idx <= size; idx++)
                    {
                        lua_rawgeti(m_lua, -1, idx);

                        if(lua_isnumber(m_lua, -1))
                            data[idx-1] = lua_tonumber(m_lua, -1);

                        else if(lua_isstring(m_lua, -1))
                        {
                            size--;

                            QString str = lua_tostring(m_lua, -1);

                            foreach(QChar ch, str)
                            {
                                size_t newSize = size + 1;
                                char* newData = new char[newSize];

                                memcpy(newData, data, size);

                                size = newSize;
                                delete [] data;
                                data = newData;

                                data[idx - 1] = ch.toAscii();
                                idx ++;
                                size ++;
                            }
                        }

                        lua_pop(m_lua, 1);
                    }

                    QVariant value = convertBytes(data, size, convertedType);

                    delete[] data;

                    QPair<QString, QVariant> pair;
                    pair.first = name;
                    pair.second = value;

                    result.append(pair);
                }

                lua_pop(m_lua, 1);
            }

            lua_pop(m_lua, 1);
        }

        lua_pop(m_lua, 2);
    }
    else
    {
        errorString = QObject::tr("No returned value");
        lua_pop(m_lua, 1);
        return result;
    }

    return result;
}

void StaticLuaLoader::init()
{
    m_lua = luaL_newstate();
    luaL_openlibs(m_lua);
}

void StaticLuaLoader::loadFile(QString fileName, QString &errorString)
{
    int ret = luaL_dofile(m_lua, fileName.toUtf8().constData());
    if (ret) {
        errorString = lua_tostring(m_lua, -1);
        return;
    }

    lua_getglobal(m_lua, "format");

    if (lua_isfunction(m_lua, -1))
    {
        if(!refs.contains(fileName))
        {
            int ref = luaL_ref(m_lua, LUA_REGISTRYINDEX);
            refs.insert(fileName, ref);
        }

        lua_pushnil(m_lua);
        lua_setglobal(m_lua, fileName.toUtf8().constData());
    }
    else
    {
        errorString = QObject::tr("There is no function format in file ") + fileName;
        return;
    }
}

QVariant StaticLuaLoader::convertBytes(char *data, int size, LogDataType type)
{
    QVariant result;
    qint64 readedSize = 0;
    qint64 pos = 0;

    if(type == UINT8_TYPE)
    {
        quint8 value = 0;

        if(!StaticRecordsReader::readArgument(data, size, pos, readedSize, value))
            return result;

        return QVariant(value);
    }
    else if(type == INT8_TYPE)
    {
        qint8 value = 0;

        if(!StaticRecordsReader::readArgument(data, size, pos, readedSize, value))
            return result;

        return QVariant(value);
    }
    else if(type == UINT16_TYPE)
    {
        quint16 value = 0;

        if(!StaticRecordsReader::readArgument(data, size, pos, readedSize, value))
            return result;

        return QVariant(value);
    }
    else if(type == UINT32_TYPE)
    {
        quint32 value = 0;

        if(!StaticRecordsReader::readArgument(data, size, pos, readedSize, value))
            return result;

        return QVariant(value);
    }
    else if(type == UINT64_TYPE)
    {
        quint64 value = 0;

        if(!StaticRecordsReader::readArgument(data, size, pos, readedSize, value))
            return result;

        return QVariant(value);
    }
    else if(type == INT32_TYPE)
    {
        qint32 value = 0;

        if(!StaticRecordsReader::readArgument(data, size, pos, readedSize, value))
            return result;

        return QVariant(value);
    }
    else if(type == BOOL_TYPE)
    {
        bool value = 0;

        if(!StaticRecordsReader::readArgument(data, size, pos, readedSize, value))
            return result;

        return QVariant(value);
    }
    else if(type == DOUBLE_TYPE)
    {
        double value = 0;

        if(!StaticRecordsReader::readArgument(data, size, pos, readedSize, value))
            return result;

        return QVariant(value);
    }
    else if(type == BYTE_ARRAY_TYPE)
    {
        quint8 argSize = 0;

        if(!StaticRecordsReader::readArgument(data, size, pos, readedSize, argSize))
            return result;

        char *value = new char[argSize + 1];
        if(!StaticRecordsReader::translateArg(data, size, pos, value, argSize))
            return false;

        pos += argSize;
        readedSize += argSize;

        QByteArray arr(value, argSize);

        delete[] value;

        return QVariant(arr);
    }
    else if(type == STRING_TYPE)
    {
        quint16 argSize = 0;

        if(!StaticRecordsReader::readArgument(data, size, pos, readedSize, argSize))
            return result;

        QString value;
        if(!StaticRecordsReader::translateArg(data, size, pos, value, argSize))
            return false;

        pos += argSize;
        readedSize += argSize;

        return QVariant(value);
    }

    return result;
}
