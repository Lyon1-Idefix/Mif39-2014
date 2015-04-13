#ifndef MESH_HPP
#define MESH_HPP

#include "Utility/utility.hpp"
#include "AssetInterfaces/interfaces.hpp"
#include "AssetGenerics/generics.hpp"

#include "Assets/structures.hpp"
#include "Assets/Material.hpp"

namespace Assets {

class Mesh;
class MeshLoader;
class MeshLibrary;
class MeshManager;

typedef QSharedPointer < Mesh > MeshPtr;
typedef QSharedPointer < MeshLoader > MeshLoaderPtr;
typedef QSharedPointer < MeshLibrary > MeshLibraryPtr;
typedef QSharedPointer < MeshManager > MeshManagerPtr;

typedef QList < QSharedPointer < Mesh > > MeshList;

class MaterialGroup :
        virtual public IResource
{
public:
    MaterialGroup ();
    MaterialGroup ( MeshPtr mesh, MaterialPtr material, Triangle* faces, unsigned int nfaces );
protected:
    virtual void _createAttributes ();
    virtual ByteBuffer _toBuffer ();
    virtual unsigned long long _fromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 );
};

class Mesh :
        virtual public SharedResource < Mesh >
{
public:
    virtual ~Mesh ();
    virtual void Usage ();
protected:
    Mesh ();
    Mesh (QString name);
    virtual void _createAttributes ();
    virtual ByteBuffer _toBuffer ();
    virtual unsigned long long _fromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 );
    friend class ResourceDescriptor < Mesh >;
};

class MeshLoader :
        virtual public SharedResourceLoader < Mesh >
{
public:
    MeshLoader ();
    virtual ~MeshLoader ();
};

class MeshManager :
        virtual public SharedResourceManager < Mesh >,
        virtual public Singleton < MeshManager >
{
public:
    MeshPtr getByUUID ( QUuid uuid );
protected:
    MeshManager();
    friend class Singleton < MeshManager >;
private:
};

}

#endif // MESH_HPP

