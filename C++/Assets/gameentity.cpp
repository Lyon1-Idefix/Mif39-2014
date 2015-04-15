#include "gameentity.hpp"

namespace Assets {

GameEntity::GameEntity ()
{
    _createAttributes ();
}

GameEntity::GameEntity (QString name)
{
    HasName::Initialize( name );
    _createAttributes ();
}

GameEntity::~GameEntity ()
{
}

void GameEntity::Usage()
{
}

void GameEntity::_createAttributes ()
{
    addIndexedAttribute < QString > ( "Semantics" );
    addIndexedAttribute < QUuid > ( "Resources" );
}

ByteBuffer GameEntity::_toBuffer() {
    unsigned long long totalSize = 0,
            index = 0;
    ByteBuffer result ( totalSize );
    return result;
}

unsigned long long GameEntity::_fromBuffer ( const ByteBuffer& buffer, unsigned long long index ) {
    unsigned long long lindex = index;
    return lindex;
}

GameEntityLoader::GameEntityLoader () {}
GameEntityLoader::~GameEntityLoader () {}


GameEntityPtr GameEntityManager::getByUUID ( QUuid uuid )
{
    return SharedResourceManager < GameEntity >::getByUUID(uuid).dynamicCast < GameEntity > ();
}

GameEntityManager::GameEntityManager() : ISharedResourceManager ( "entity/" ), SharedResourceManager < GameEntity > ( "entity/" ) {
    HasUUID::InitializeUUID( "entity/" );
}

}

