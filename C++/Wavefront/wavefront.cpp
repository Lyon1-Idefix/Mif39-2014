
#include "Wavefront/wavefront.hpp"

void __attachWavefront(void) __attribute__((constructor));
void __detachWavefront(void) __attribute__((destructor));

void __attachWavefront(void)
{
    static bool initialized = false;
    if ( ! initialized ) {
    std::cout << "Attaching Wavefront library" << std::endl;
    ResourceHolder::RegisterImplementation < WavefrontMaterial, Assets::Material > ( "WavefrontMaterial" );

    std::cout << "Initialize Wavefront Loaders" << std::endl;
    QMimeDatabase db;
    foreach ( QMimeType type, db.allMimeTypes() ) {
        if ( type.name() == "material/wavefront-mtl" ) {
            ResourceHolder::RegisterLoader < WavefrontMaterial, WavefrontLoaderMTL > ( type );
        }
        if ( type.name() == "geometry/wavefront-obj" ) {
            ResourceHolder::RegisterLoader < Assets::Mesh, WavefrontLoaderOBJ > ( type );
        }
    }
    initialized = true;
    }
}

void __detachWavefront(void)
{
    std::cout << "Detaching Wavefront library" << std::endl;
}
