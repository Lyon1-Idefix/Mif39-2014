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

// generic ByteBuffer conversion
template < class InnerType > unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const InnerType& value );
template < class InnerType > unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, InnerType& value );
template < class InnerType > ByteBuffer toBuffer ( const InnerType& value );

// generic ByteBuffer conversion
template < class InnerType > unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const InnerType*& value );
template < class InnerType > unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, InnerType*& value );
template < class InnerType > ByteBuffer toBuffer ( const InnerType*& value );

// generic ByteBuffer conversion (C/C++ arrays)
template < class InnerType > unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const InnerType* value, const unsigned long long length );
template < class InnerType > unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, InnerType* value, const unsigned long long length );
template < class InnerType > ByteBuffer toBuffer ( const InnerType* value, const unsigned long long length );

///////////////////////////////////////

// specialized ByteBuffer conversion (QT lists)
template < class InnerType > unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const QList < InnerType >& value );
template < class InnerType > unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, const QList < InnerType >& value );
template < class InnerType > ByteBuffer toBuffer ( const QList < InnerType >& value );

// specialized ByteBuffer conversion (QT lists of pointers)
template < class InnerType > unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const QList < InnerType* >& value );
template < class InnerType > unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, const QList < InnerType* >& value );
template < class InnerType > ByteBuffer toBuffer ( const QList < InnerType* >& value );

///////////////////////////////////////

template <> unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const QUuid& value );
template <> unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, QUuid& value );
template <> ByteBuffer toBuffer ( const QUuid& value );

template <> unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const QString& value );
template <> unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, QString& value );
template <> ByteBuffer toBuffer ( const QString& value );

///////////////////////////////////////

#include "bytebuffer.inline.hpp"

#endif // BYTEBUFFER_H
