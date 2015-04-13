
#include "Wavefront/wavefront.hpp"

void __attach(void) __attribute__((constructor));
void __detach(void) __attribute__((destructor));

void __attach(void)
{
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
}

void __detach(void)
{
    std::cout << "Detaching Wavefront library" << std::endl;
}
