#include "Utility/hasuuid.hpp"

HasUUID::HasUUID ()
{
}

void HasUUID::InitializeUUID ( QString reference )
{
    //mUuid = UUIDManager::createUUID (reference);
}

#define A 54059 /* a prime */
#define B 76963 /* another prime */
#define C 86969 /* yet another prime */
#define D 132241

typedef union {
    uint vui;
    struct {
        ushort s1;
        ushort s2;
    } vus;
    struct {
        uchar c1;
        uchar c2;
        uchar c3;
        uchar c4;
    } vuc;
} Key;

uint genhash(const char* s, uint a, uint b)
{
   uint h = 31 /* also prime */;
   while (*s) {
     h = (h * a) ^ (s[0] * b);
     s++;
   }
   return h; // or return h % C;
}

QUuid UUIDManager::createUUID ( QString reference )
{
    if ( mapNameToUuid.find ( reference ) == mapNameToUuid.end () ) {
        Key key1, key2, key3, key4;
        key1.vui = genhash(reference.toStdString().c_str(), A,B);
        key2.vui = genhash(reference.toStdString().c_str(), B,C);
        key3.vui = genhash(reference.toStdString().c_str(), C,C);
        key4.vui = genhash(reference.toStdString().c_str(), D,A);
        QUuid uuid ( key1.vui, key2.vus.s1, key2.vus.s2, key3.vuc.c1, key3.vuc.c2, key3.vuc.c3, key3.vuc.c4, key4.vuc.c1, key4.vuc.c2, key4.vuc.c3, key4.vuc.c4);
        mapNameToUuid [ reference ] = uuid;
        mapUuidToName [ uuid ] = reference;
    }
    return mapNameToUuid [ reference ];
}

QMap < QUuid, QString > UUIDManager::mapUuidToName;
QMap < QString, QUuid > UUIDManager::mapNameToUuid;
