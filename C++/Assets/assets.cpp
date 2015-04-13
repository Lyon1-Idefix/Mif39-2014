

#include "Assets/assets.hpp"

void __attachAssets(void) __attribute__((constructor));
void __detachAssets(void) __attribute__((destructor));

void __attachAssets(void)
{
    static bool initialized = false;
    if ( ! initialized ) {
        std::cout << "Attaching Assets library" << std::endl;
        UUIDManager::initialize();

        std::cout << "Initialize Assets Managers" << std::endl;
        ResourceHolder::RegisterInterface < Assets::Image, Assets::ImageManager > ( "Image" );
        ResourceHolder::RegisterInterface < Assets::Material, Assets::MaterialManager > ( "Material" );
        ResourceHolder::RegisterInterface < Assets::Mesh, Assets::MeshManager > ( "Mesh" );
        ResourceHolder::RegisterInterface < Assets::Behaviour, Assets::BehaviourManager > ( "Behaviour" );

        std::cout << "Initialize Implementation Resource" << std::endl;
        ResourceHolder::RegisterImplementation < Assets::Image, Assets::Image > ( "Image" );
        ResourceHolder::RegisterImplementation < Assets::Mesh, Assets::Mesh > ( "Mesh" );
        initialized = true;
    }
}

void __detachAssets(void)
{
    std::cout << "Detaching Assets library" << std::endl;
}
