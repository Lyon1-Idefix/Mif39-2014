#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

#include "Utility/includes.hpp"

/*
 * format pour les ressources :
 *  - UUID du type
 *  - données propres de la ressource
 *      - UUID de la ressource
 *      - nom de la ressource
 *      - ...
 *  - données des attributs de la ressource
 */

class ByteBuffer
{
public:
    ByteBuffer ( unsigned long long lg = 0 );
    ByteBuffer ( unsigned char* data, unsigned long long lg = 0 );
    ByteBuffer ( const ByteBuffer& buf );
    ~ByteBuffer ();
    //
    ByteBuffer& operator= ( const ByteBuffer& buffer );
    //
    inline unsigned long long getLength () { return length; }
    inline const unsigned long long getLength () const { return length; }
    inline unsigned char* getData () { return buffer; }
    inline const unsigned char* getData () const { return buffer; }
    //
    void append ( const ByteBuffer& buf );
    void reserve ( unsigned long long size );
    void trim ();
protected:
    unsigned long long length;
    unsigned long long reserved;
    unsigned char* buffer;
    bool isPropertyOf;
};

///////////////////////////////////////

template < class InnerType > unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const InnerType& value )
{
    if ( ( index + sizeof ( InnerType ) ) > buffer.getLength() ) std::cout << "****** SIZE ERROR *******" << std::endl;
    memcpy ( buffer.getData() + index, (void*) & value, sizeof ( InnerType ) );
    return index + sizeof ( InnerType );
}

template < class InnerType > unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, InnerType& value )
{
    memcpy ( (void*) & value, buffer.getData() + index, sizeof ( InnerType ) );
    return index + sizeof ( InnerType );
}

///////////////////////////////////////

template < class InnerType > unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const InnerType* value, const unsigned long long length )
{
    if ( ( index + length * sizeof ( InnerType ) ) > buffer.getLength() ) std::cout << "****** SIZE ERROR *******" << std::endl;
    memcpy ( buffer.getData() + index, (void*) value, length * sizeof ( InnerType ) );
    return index + length * sizeof ( InnerType );
}

template < class InnerType > unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, InnerType* value, const unsigned long long length )
{
    memcpy ( (void*) value, buffer.getData() + index, length * sizeof ( InnerType ) );
    return index + length * sizeof ( InnerType );
}

///////////////////////////////////////

template < class InnerType > ByteBuffer toBuffer ( const InnerType& value ) {
    ByteBuffer result ( sizeof ( InnerType ) );
    unsigned int nindex = toBuffer ( result, 0, value );
    return result;
}

template < class InnerType > ByteBuffer toBuffer ( const InnerType* value, const unsigned long long length ) {
    ByteBuffer result ( length * sizeof ( InnerType ) );
    unsigned int nindex = toBuffer ( result, 0, value, length );
    return result;
}

///////////////////////////////////////

template <> unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const QUuid& value );
template <> unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, QUuid& value );
template <> ByteBuffer toBuffer ( const QUuid& value );

template <> unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const QString& value );
template <> unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, QString& value );
template <> ByteBuffer toBuffer ( const QString& value );

#endif // BYTEBUFFER_H
