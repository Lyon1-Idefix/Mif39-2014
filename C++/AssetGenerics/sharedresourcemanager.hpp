#ifndef SHAREDRESOURCEMANAGER_HPP
#define SHAREDRESOURCEMANAGER_HPP

#include "AssetGenerics/internals.hpp"

template < class ResourceType > class SharedResourceManager :
        virtual public ISharedResourceManager
{
protected:
    SharedResourceManager(QString pattern) :
        ISharedResourceManager ( pattern )
    {
    }

    QSharedPointer< SharedResource<ResourceType> > getByUUID ( QUuid uuid )
    {
        return ISharedResourceManager::getByUUID(uuid).template dynamicCast < SharedResource<ResourceType> > ();
    }
};

#endif // SHAREDRESOURCEMANAGER_HPP

