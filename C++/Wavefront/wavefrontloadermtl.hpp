#ifndef WAVEFRONTLOADERMTL_H
#define WAVEFRONTLOADERMTL_H

#include "Assets/assets.hpp"

#include "Wavefront/wavefrontmaterial.hpp"

class WavefrontLoaderMTL :
        virtual public Assets::MaterialLoader,
        virtual public Singleton < WavefrontLoaderMTL >
{
public:
    virtual ~WavefrontLoaderMTL ();
protected:
    WavefrontLoaderMTL ();

    virtual SharedResourceList __load ( FileDescriptor filename );
    friend class Singleton < WavefrontLoaderMTL >;
private:
};

#endif // WAVEFRONTLOADERMTL_H
