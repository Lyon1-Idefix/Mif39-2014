#include "Utility/hasuuid.hpp"

HasUUID::HasUUID ()
{
}

void HasUUID::InitializeUUID ( QString reference )
{
    mUuid = UUIDManager::createUUID (reference);
}

QUuid UUIDManager::createUUID ( QString reference )
{
    if ( mapNameToUuid.find ( reference ) == mapNameToUuid.end () ) {
        QUuid uuid = QUuid::createUuid();
        mapNameToUuid [ reference ] = uuid;
        mapUuidToName [ uuid ] = reference;
    }
    return mapNameToUuid [ reference ];
}

QMap < QUuid, QString > UUIDManager::mapUuidToName;
QMap < QString, QUuid > UUIDManager::mapNameToUuid;
