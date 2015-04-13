#ifndef SHAREDRESOURCE_HPP
#define SHAREDRESOURCE_HPP

#include "AssetGenerics/internals.hpp"
#include "AssetGenerics/resourceattribute.hpp"

template <class ResourceType> class SharedResource :
        virtual public ISharedResource
{
public:
    SharedResource () {}
    SharedResource (QString name) :
        ISharedResource(name)
    {
    }
    virtual ~SharedResource ()
    {
    }
};

#endif // SHAREDRESOURCE_HPP

