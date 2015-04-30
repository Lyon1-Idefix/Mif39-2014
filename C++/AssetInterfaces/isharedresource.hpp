#ifndef ISHAREDRESOURCE_HPP
#define ISHAREDRESOURCE_HPP

#include "AssetInterfaces/internals.hpp"
#include "AssetInterfaces/iresourceattribute.hpp"
#include "AssetInterfaces/attributecontainer.hpp"

class ISharedResource :
        virtual public HasUUID,
        virtual public HasName,
        virtual public AttributeContainer
{
public:
    ISharedResource () {}
    ISharedResource (QString name);
    virtual ~ISharedResource ();
    //
    virtual void Usage () = 0;

    virtual ByteBuffer convertToBuffer () {
        ByteBuffer uuid = toBuffer ( mUuid );
        ByteBuffer name = toBuffer ( mName );
        ByteBuffer data = _toBuffer();
        unsigned long long index = 0;
        ByteBuffer buffer ( uuid.getLength() + name.getLength() + data.getLength() );
        index = ::toBuffer ( buffer, index, uuid.getData(), uuid.getLength() );
        index = ::toBuffer ( buffer, index, name.getData(), name.getLength() );
        index = ::toBuffer ( buffer, index, data.getData(), data.getLength() );
        return buffer;
    }
    virtual unsigned long long convertFromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 ) {
        unsigned long long lindex = index;
        lindex = ::fromBuffer ( buffer, lindex, mUuid );
        lindex = ::fromBuffer ( buffer, lindex, mName );
        lindex = _fromBuffer(buffer,lindex);
        return lindex;
    }

    virtual ByteBuffer _toBuffer () = 0;
    virtual unsigned long long _fromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 ) = 0;
    QUuid mTypeID;
protected:
    FileDescriptor mFromFile;
    friend class ResourceHolder;
};

#endif // ISHAREDRESOURCE_HPP

