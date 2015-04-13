#ifndef DATA_MATERIAL_HPP
#define DATA_MATERIAL_HPP
#include "Utility/utility.hpp"

#include "AssetInterfaces/interfaces.hpp"
#include "AssetGenerics/generics.hpp"

#include "Assets/structures.hpp"
#include "Assets/Image.hpp"

namespace Assets {

class Material;
class MaterialLoader;
class MaterialManager;

typedef QSharedPointer < Material > MaterialPtr;
typedef QSharedPointer < MaterialLoader > MaterialLoaderPtr;
typedef QSharedPointer < MaterialManager > MaterialManagerPtr;

typedef QList < QSharedPointer < Material > > MaterialList;

class MaterialAttribute :
        virtual public IResource
{
public:
    MaterialAttribute () {}
};

class Material :
        virtual public SharedResource < Material >
{
public:
    virtual ~Material ();
protected:
    Material () {}
    Material (QString name);
    friend class ResourceDescriptor < Material >;
    /*
private:
    virtual void Usage () {}
    virtual ByteBuffer _toBuffer () {ByteBuffer tmp; return tmp;}
    virtual void _fromBuffer ( const ByteBuffer& buffer ) {}
    virtual void _createAttributes () {}
    */
};

class MaterialLoader :
        virtual public SharedResourceLoader < Material >
{
public:
    MaterialLoader ();
    virtual ~MaterialLoader ();
};

class MaterialManager :
        virtual public SharedResourceManager < Material >,
        virtual public Singleton < MaterialManager >
{
public:
    MaterialPtr getByUUID ( QUuid uuid );
protected:
    MaterialManager();
    friend class Singleton < MaterialManager >;
private:
};

}

#endif // MATERIAL_HPP

