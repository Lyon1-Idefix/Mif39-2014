#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "AssetInterfaces/interfaces.hpp"
#include "AssetGenerics/generics.hpp"

namespace Assets {

class GameEntity;
class GameEntityLoader;
class GameEntityManager;

typedef QSharedPointer < GameEntity > GameEntityPtr;
typedef QSharedPointer < GameEntityLoader > GameEntityLoaderPtr;
typedef QSharedPointer < GameEntityManager > GameEntityManagerPtr;

typedef QList < QSharedPointer < GameEntity > > GameEntityList;

class GameEntityAttribute :
        virtual public IResource
{
public:
    GameEntityAttribute () {}
};

class GameEntity :
        virtual public SharedResource < GameEntity >
{
public:
    virtual ~GameEntity ();
    virtual void Usage ();
protected:
    GameEntity ();
    GameEntity (QString name);
    virtual void _createAttributes ();
    virtual ByteBuffer _toBuffer ();
    virtual unsigned long long _fromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 );
    friend class ResourceDescriptor < GameEntity >;
};

class GameEntityLoader :
        virtual public SharedResourceLoader < GameEntity >
{
public:
    GameEntityLoader ();
    virtual ~GameEntityLoader ();
};

class GameEntityManager :
        virtual public SharedResourceManager < GameEntity >,
        virtual public Singleton < GameEntityManager >
{
public:
    GameEntityPtr getByUUID ( QUuid uuid );
protected:
    GameEntityManager();
    friend class Singleton < GameEntityManager >;
private:
};

}

#endif // GAMEENTITY_H
