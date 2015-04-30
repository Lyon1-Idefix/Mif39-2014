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

class GameEntityElement :
        virtual public IResource
{
public:
    GameEntityElement ();
    GameEntityElement ( GameEntityPtr entity, QString name );
protected:
    virtual void _createAttributes ();
    virtual ByteBuffer _toBuffer ();
    virtual unsigned long long _fromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 );
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
        virtual public SharedResourceLoader < GameEntity >,
        virtual public Singleton < GameEntityLoader >
{
public:
    GameEntityLoader ();
    virtual ~GameEntityLoader ();
    virtual SharedResourceList __load ( FileDescriptor filename );
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
