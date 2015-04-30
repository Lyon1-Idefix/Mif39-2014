#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "AssetInterfaces/interfaces.hpp"
#include "AssetGenerics/generics.hpp"

namespace Assets {

class GameObject;
class GameObjectLoader;
class GameObjectManager;

typedef QSharedPointer < GameObject > GameObjectPtr;
typedef QSharedPointer < GameObjectLoader > GameObjectLoaderPtr;
typedef QSharedPointer < GameObjectManager > GameObjectManagerPtr;

typedef QList < QSharedPointer < GameObject > > GameObjectList;

class GameObject :
        virtual public SharedResource < GameObject >
{
public:
    virtual ~GameObject ();
    virtual void Usage ();
protected:
    GameObject ();
    GameObject (QString name);
    virtual void _createAttributes ();
    virtual ByteBuffer _toBuffer ();
    virtual unsigned long long _fromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 );
    friend class ResourceDescriptor < GameObject >;
};

class GameObjectManager :
        virtual public SharedResourceManager < GameObject >,
        virtual public Singleton < GameObjectManager >
{
public:
    GameObjectPtr getByUUID ( QUuid uuid );
protected:
    GameObjectManager();
    friend class Singleton < GameObjectManager >;
private:
};

}

#endif // GAMEOBJECT_H
