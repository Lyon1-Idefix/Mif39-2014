#include "Utility/bytebuffer.hpp"

#define BlockSize 1024

unsigned char* allocateBuffer ( ByteBuffer* user, unsigned long long length ) {
    unsigned char* buf = new unsigned char [ length ];
    //std::cout << "Buffer Allocation from (" << user << ") with length (" << length << ") got (" << ((void*)buf) << ")" << std::endl;
    return buf;
}

void freeBuffer ( ByteBuffer* user, unsigned char* buf ) {
    //std::cout << "Buffer Release from (" << user << ") got (" << ((void*)buf) << ")" << std::endl;
    delete [] buf;
}

ByteBuffer::ByteBuffer ( unsigned char* data, unsigned long long lg ) :
    length ( lg ),
    reserved ( lg ),
    buffer ( data ),
    isPropertyOf ( false )
{
}

ByteBuffer::ByteBuffer ( unsigned long long lg ) :
    length ( lg ),
    reserved ( lg ),
    buffer ( NULL ),
    isPropertyOf ( true )
{
    if ( lg > 0 )
        buffer = allocateBuffer ( this, lg );
}

ByteBuffer::ByteBuffer ( const ByteBuffer& buf ) :
    length ( buf.length ),
    reserved ( buf. reserved ),
    isPropertyOf ( true )
{
    if ( length > 0 ) {
        buffer = allocateBuffer ( this, length );
        memcpy ( buffer, buf.buffer, length );
    }
}

ByteBuffer::~ByteBuffer ()
{
    if ( isPropertyOf && ( buffer != NULL ) ) {
        freeBuffer ( this, buffer );
    }
}
ByteBuffer& ByteBuffer::operator= ( const ByteBuffer& buf )
{
    if ( this != & buf ) {
        if ( isPropertyOf && ( buffer != NULL ) ) {
            freeBuffer ( this, buffer );
        }
        length = reserved = buf.length;
        buffer = NULL;
        isPropertyOf = true;
        if ( length > 0 ) {
            buffer = allocateBuffer ( this, length );
            memcpy ( buffer, buf.buffer, length );
        }
    }
    return *this;
}

void ByteBuffer::append ( const ByteBuffer& buf )
{
    if ( length + buf.length > reserved ) {
        unsigned char* tmp = allocateBuffer ( this, length + buf.length );
        memcpy ( tmp, buffer, length );
        if ( isPropertyOf && ( buffer != NULL ) )
            freeBuffer ( this, buffer );
        reserved += buf.length;
        buffer = tmp;
        isPropertyOf = true;
    }
    memcpy ( buffer + length, buf.buffer, buf.length );
    length += buf.length;
}

void ByteBuffer::trim ()
{
    if ( length < reserved ) {
        unsigned char* tmp = allocateBuffer ( this, length );
        memcpy ( tmp, buffer, length );
        reserved = length;
        if ( isPropertyOf && ( buffer != NULL ) )
            freeBuffer ( this, buffer );
        buffer = tmp;
        isPropertyOf = true;
    }
}

void ByteBuffer::reserve ( unsigned long long size )
{
    if ( size > reserved ) {
        if ( isPropertyOf && ( buffer != NULL ) )
            freeBuffer ( this, buffer );
        buffer = allocateBuffer ( this, size );
        reserved = size;
        isPropertyOf = true;
    }
    length = reserved;
}

///////////////////////////////////////////////////////////

template <> unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const QString& value )
{
    unsigned long long tmp = 0;
    unsigned long long lg = value.length();
    tmp = toBuffer ( buffer, index, lg );
    tmp = toBuffer ( buffer, tmp, value.toStdString().c_str(), lg );
    return tmp;
}

template <> unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, QString& value )
{
    unsigned long long tmp = 0;
    unsigned long long lg;
    tmp = fromBuffer ( buffer, index, lg );
    unsigned char str [ lg + 1 ];
    tmp = fromBuffer ( buffer, tmp, str, lg );
    str [ lg ] = 0;
    value = (char*) str;
    return tmp;
}

template <> ByteBuffer toBuffer ( const QString& value )
{
    ByteBuffer result ( sizeof ( unsigned long long ) + value.length() );
    unsigned int nindex = toBuffer ( result, 0, value );
    return result;
}

///////////////////////////////////////////////////////////

template <> unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const QUuid& value )
{
    unsigned long long tmp = 0;
    QByteArray uuid = value.toByteArray();
    tmp = ::toBuffer ( buffer, index, uuid.data(), uuid.size() );
    return tmp;
}

template <> unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, QUuid& value )
{
    unsigned long long tmp = 0;
    QByteArray uuid = value.toByteArray();
    tmp = ::fromBuffer ( buffer, index, uuid.data(), uuid.size() );
    value = QUuid (uuid);
    return tmp;
}

template <> ByteBuffer toBuffer ( const QUuid& value )
{
    QByteArray uuid = value.toByteArray();
    ByteBuffer result ( uuid.size() );
    unsigned long long nindex = toBuffer ( result, 0, value );
    return result;
}
