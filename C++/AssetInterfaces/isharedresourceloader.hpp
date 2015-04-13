#ifndef ISHAREDRESOURCELOADER_HPP
#define ISHAREDRESOURCELOADER_HPP

#include "AssetInterfaces/internals.hpp"

class ISharedResourceLoader :
        virtual public HasUUID,
        virtual public HasName
{
public:
    ISharedResourceLoader () {}
    virtual ~ISharedResourceLoader () {}
    inline SharedResourceList load (FileDescriptor filename) {
        return __load ( filename );
    }
protected:
    virtual SharedResourceList __load ( FileDescriptor filename ) = 0;
};

#endif // ISHAREDRESOURCELOADER_HPP

