#ifndef WORLD_H
#define WORLD_H

#include "AssetInterfaces/interfaces.hpp"
#include "AssetGenerics/generics.hpp"

namespace Assets {

class World;
class WorldLoader;
class WorldManager;

typedef QSharedPointer < World > WorldPtr;
typedef QSharedPointer < WorldLoader > WorldLoaderPtr;
typedef QSharedPointer < WorldManager > WorldManagerPtr;

typedef QList < QSharedPointer < World > > WorldList;

class SpawnPoint :
        virtual public IResource
{
public:
    SpawnPoint ();
    SpawnPoint ( WorldPtr world, QString name );
protected:
    virtual void _createAttributes ();
    virtual ByteBuffer _toBuffer ();
    virtual unsigned long long _fromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 );
};

class World :
        virtual public SharedResource < World >
{
public:
    virtual ~World ();
    virtual void Usage ();
protected:
    World ();
    World (QString name);
    virtual void _createAttributes ();
    virtual ByteBuffer _toBuffer ();
    virtual unsigned long long _fromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 );
    friend class ResourceDescriptor < World >;
};

class WorldLoader :
        virtual public SharedResourceLoader < World >,
        virtual public Singleton < WorldLoader >
{
public:
    WorldLoader ();
    virtual ~WorldLoader ();
    virtual SharedResourceList __load ( FileDescriptor filename );
};

class WorldManager :
        virtual public SharedResourceManager < World >,
        virtual public Singleton < WorldManager >
{
public:
    WorldPtr getByUUID ( QUuid uuid );
protected:
    WorldManager();
    friend class Singleton < WorldManager >;
private:
};

}

#endif // WORLD_H
