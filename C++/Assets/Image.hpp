#ifndef DATA_IMAGE_HPP
#define DATA_IMAGE_HPP

#include "Utility/utility.hpp"
#include "AssetInterfaces/interfaces.hpp"
#include "AssetGenerics/generics.hpp"

#include "Assets/structures.hpp"

namespace Assets {

class Image;
class ImageLoader;
class ImageLibrary;
class ImageManager;

typedef QSharedPointer < Image > ImagePtr;
typedef QSharedPointer < ImageLoader > ImageLoaderPtr;
typedef QSharedPointer < ImageLibrary > ImageLibraryPtr;
typedef QSharedPointer < ImageManager > ImageManagerPtr;

typedef QList < QSharedPointer < Image > > ImageList;

///
/// \brief The Image class
///
class Image :
        virtual public SharedResource < Image >
{
public:
    virtual ~Image ();
    virtual void Usage ();
protected:
    Image ();
    Image (QString name);
    virtual void _createAttributes ();
    virtual ByteBuffer _toBuffer ();
    virtual unsigned long long _fromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 );
    friend class ResourceDescriptor < Image >;
};

///
/// \brief The ImageLoader class
///
class ImageLoader :
        virtual public SharedResourceLoader < Image >
{
public:
    ImageLoader ();
    virtual ~ImageLoader ();
};

///
/// \brief The ImageManager class
///
class ImageManager :
        virtual public SharedResourceManager < Image >,
        virtual public Singleton < ImageManager >
{
public:
    ImagePtr getByUUID ( QUuid uuid );
protected:
    ImageManager();
    friend class Singleton < ImageManager >;
private:
};

}

#endif
