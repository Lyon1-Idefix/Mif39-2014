#include "AssetInterfaces/isharedresource.hpp"

ISharedResource::ISharedResource (QString name)
{
    HasName::Initialize(name);
}

ISharedResource::~ISharedResource ()
{
}
