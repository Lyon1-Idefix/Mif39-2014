#ifndef WAVEFRONTLOADEROBJ_H
#define WAVEFRONTLOADEROBJ_H

#include "Assets/assets.hpp"

class WavefrontLoaderOBJ :
        virtual public Assets::MeshLoader,
        virtual public Singleton < WavefrontLoaderOBJ >
{
public:
    virtual ~WavefrontLoaderOBJ ();
protected:
    WavefrontLoaderOBJ ();

    virtual SharedResourceList __load ( FileDescriptor filename );
    friend class Singleton < WavefrontLoaderOBJ >;
private:
};

#endif // WAVEFRONTLOADEROBJ_H
