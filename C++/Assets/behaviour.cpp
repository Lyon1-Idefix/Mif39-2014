#include "behaviour.hpp"

namespace Assets {

Behaviour::Behaviour (QString name)
{
    HasName::Initialize ( name );
}

Behaviour::~Behaviour ()
{
}

BehaviourLoader::BehaviourLoader () {
}

BehaviourLoader::~BehaviourLoader () {
}

BehaviourPtr BehaviourManager::getByUUID ( QUuid uuid )
{
    return SharedResourceManager < Behaviour >::getByUUID(uuid).dynamicCast < Behaviour > ();
}

BehaviourManager::BehaviourManager() :
    ISharedResourceManager ( "behaviour/" ), SharedResourceManager < Behaviour > ( "behaviour/" )
{
    HasUUID::InitializeUUID ( "behaviour/" );
}

}
