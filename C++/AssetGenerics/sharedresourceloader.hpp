#ifndef SHAREDRESOURCELOADER_CPP
#define SHAREDRESOURCELOADER_CPP

#include "AssetGenerics/internals.hpp"

template < class ResourceType > class SharedResourceLoader :
        virtual public ISharedResourceLoader
{
public:
    SharedResourceLoader () {}
    virtual ~SharedResourceLoader () {}
};

#endif // SHAREDRESOURCELOADER_CPP

