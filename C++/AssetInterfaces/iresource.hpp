#ifndef IRESOURCE_H
#define IRESOURCE_H

#include "AssetInterfaces/internals.hpp"
#include "AssetInterfaces/attributecontainer.hpp"

class IResource :
        virtual public HasName,
        virtual public AttributeContainer
{
public:
    virtual ~IResource ();
    virtual ByteBuffer convertToBuffer () {
        ByteBuffer name = toBuffer ( mName );
        ByteBuffer data = _toBuffer();
        unsigned long long index = 0;
        ByteBuffer buffer ( name.getLength() + data.getLength() );
        index = ::toBuffer ( buffer, index, name.getData(), name.getLength() );
        index = ::toBuffer ( buffer, index, data.getData(), data.getLength() );
        return buffer;
    }
    virtual unsigned long long convertFromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 ) {
        unsigned long long lindex = index;
        lindex = ::fromBuffer ( buffer, lindex, mName );
        lindex = _fromBuffer(buffer,lindex);
        return lindex;
    }

protected:
    IResource ();
    virtual ByteBuffer _toBuffer () = 0;
    virtual unsigned long long _fromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 ) = 0;
};

#endif // IRESOURCE_H
