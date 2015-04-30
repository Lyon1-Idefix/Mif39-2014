

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
        ResourceHolder::RegisterInterface < Assets::Light, Assets::LightManager > ( "Light" );
        ResourceHolder::RegisterInterface < Assets::GameEntity, Assets::GameEntityManager > ( "GameEntity" );
        ResourceHolder::RegisterInterface < Assets::GameObject, Assets::GameObjectManager > ( "GameObject" );
        ResourceHolder::RegisterInterface < Assets::WorldCell, Assets::WorldCellManager > ( "WorldCell" );
        ResourceHolder::RegisterInterface < Assets::World, Assets::WorldManager > ( "World" );

        std::cout << "Initialize Implementation Resource" << std::endl;
        ResourceHolder::RegisterImplementation < Assets::Image, Assets::Image > ( "Image" );
        ResourceHolder::RegisterImplementation < Assets::Mesh, Assets::Mesh > ( "Mesh" );
        ResourceHolder::RegisterImplementation < Assets::Light, Assets::Light > ( "Light" );
        ResourceHolder::RegisterImplementation < Assets::GameEntity, Assets::GameEntity > ( "GameEntity" );
        ResourceHolder::RegisterImplementation < Assets::GameObject, Assets::GameObject > ( "GameObject" );
        ResourceHolder::RegisterImplementation < Assets::WorldCell, Assets::WorldCell > ( "WorldCell" );
        ResourceHolder::RegisterImplementation < Assets::World, Assets::World > ( "World" );

        QMimeDatabase db;
        foreach ( QMimeType type, db.allMimeTypes() ) {
            if ( type.name() == "world/lights" ) {
                ResourceHolder::RegisterLoader < Assets::Light, Assets::LightLoader > ( type );
            }
            if ( type.name() == "world/entities" ) {
                ResourceHolder::RegisterLoader < Assets::GameEntity, Assets::GameEntityLoader > ( type );
            }
            if ( type.name() == "world/world" ) {
                ResourceHolder::RegisterLoader < Assets::World, Assets::WorldLoader > ( type );
            }
        }
        initialized = true;
    }
}

void __detachAssets(void)
{
    std::cout << "Detaching Assets library" << std::endl;
}
