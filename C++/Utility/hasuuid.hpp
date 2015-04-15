#ifndef HASUUID_H
#define HASUUID_H

#include "Utility/includes.hpp"

class HasUUID
{
public:
    inline QUuid getUUID () { return mUuid; }
    inline void setUUID (QUuid value) { mUuid = value; }
    inline const QUuid getUUID () const { return mUuid; }
protected:
    HasUUID ();
    void InitializeUUID ( QString reference );
    QUuid mUuid;
    friend class ResourceHolder;
};

class UUIDManager
{
public:
    static void initialize () { std::cout << "UUIDManager::initialize ()" << std::endl; }
    static QUuid createUUID ( QString reference );
protected:
    static QMap < QUuid, QString > mapUuidToName;
    static QMap < QString, QUuid > mapNameToUuid;
};

#endif // HASUUID_H
