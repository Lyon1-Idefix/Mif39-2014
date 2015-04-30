#ifndef LIGHT_H
#define LIGHT_H

#include "Utility/utility.hpp"
#include "AssetInterfaces/interfaces.hpp"
#include "AssetGenerics/generics.hpp"

#include "Assets/structures.hpp"

namespace Assets {

class Light;
class LightLoader;
class LightLibrary;
class LightManager;

typedef QSharedPointer < Light > LightPtr;
typedef QSharedPointer < LightLoader > LightLoaderPtr;
typedef QSharedPointer < LightLibrary > LightLibraryPtr;
typedef QSharedPointer < LightManager > LightManagerPtr;

typedef QList < QSharedPointer < Light > > LightList;

class Light :
        virtual public SharedResource < Light >
{
public:
    typedef enum { Point, Direction, Surface } Type;
    virtual ~Light ();
    virtual void Usage ();
protected:
    Light ();
    Light (QString name);
    virtual void _createAttributes ();
    virtual ByteBuffer _toBuffer ();
    virtual unsigned long long _fromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 );
    friend class ResourceDescriptor < Light >;
};

class LightLoader :
        virtual public SharedResourceLoader < Light >,
        virtual public Singleton < LightLoader >
{
public:
    LightLoader ();
    virtual ~LightLoader ();
    virtual SharedResourceList __load ( FileDescriptor filename );
};

class LightManager :
        virtual public SharedResourceManager < Light >,
        virtual public Singleton < LightManager >
{
public:
    LightPtr getByUUID ( QUuid uuid );
protected:
    LightManager();
    friend class Singleton < LightManager >;
private:
};

}

#endif // LIGHT_H
