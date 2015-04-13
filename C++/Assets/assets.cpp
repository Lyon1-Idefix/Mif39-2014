

#include "Assets/assets.hpp"

void __attach(void) __attribute__((constructor));
void __detach(void) __attribute__((destructor));

void __attach(void)
{
    static bool initialized = false;
    if ( ! initialized ) {
        std::cout << "Attaching Assets library" << std::endl;
        UUIDManager::initialize();

        std::cout << "Initialize Assets Managers" << std::endl;
        ResourceHolder::RegisterInterface < Assets::Image, Assets::ImageManager > ( "Image" );
        ResourceHolder::RegisterInterface < Assets::Material, Assets::MaterialManager > ( "Material" );
        ResourceHolder::RegisterInterface < Assets::Mesh, Assets::MeshManager > ( "Mesh" );

        std::cout << "Initialize Implementation Resource" << std::endl;
        ResourceHolder::RegisterImplementation < Assets::Image, Assets::Image > ( "Image" );
        ResourceHolder::RegisterImplementation < Assets::Mesh, Assets::Mesh > ( "Mesh" );
        initialized = true;
    }
}

void __detach(void)
{
    std::cout << "Detaching Assets library" << std::endl;
}
