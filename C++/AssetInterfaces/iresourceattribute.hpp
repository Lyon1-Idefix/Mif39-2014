#ifndef IRESOURCEATTRIBUTE_HPP
#define IRESOURCEATTRIBUTE_HPP

#include "Utility/utility.hpp"

////////////////////////////////////////////////////////////
///
///
class IResourceAttribute :
        virtual public HasName
{
public:
    virtual ~IResourceAttribute () {}
    template <class AttributeType> IResourceAttribute& operator= ( const AttributeType& value );
    //
    template <class AttributeType> void setValue ( AttributeType value );
    template <class AttributeType> AttributeType& getValue ();
    template <class AttributeType> const AttributeType& getValue () const;
    //
    template <class AttributeType> void addValue ( AttributeType value );
    template <class AttributeType> AttributeType& getValue (int index);
    template <class AttributeType> const AttributeType& getValue (int index) const;
    //
    template <class AttributeType> void addValue ( QString name, AttributeType value );
    template <class AttributeType> AttributeType& getValue (QString name);
    template <class AttributeType> const AttributeType& getValue (QString name) const;
    //
    template <class AttributeType> unsigned int getSize () const;
    template < class AttributeType > QList < QString > getKeys () const;
    //
    virtual ByteBuffer convertToBuffer () = 0;
    virtual unsigned long long convertToBuffer ( ByteBuffer& buffer, unsigned long long index ) = 0;
    virtual unsigned long long convertFromBuffer ( const ByteBuffer& buffer, unsigned long long index ) = 0;
protected:
    IResourceAttribute (QString name) {
        HasName::Initialize(name);
    }
};

////////////////////////////////////////////////////////////
///
///
template < class AttributeType > class ResourceAttribute :
        virtual public IResourceAttribute
{
public:
    ResourceAttribute (QString name, AttributeType value) :
        IResourceAttribute ( name ),
        mValue ( value )
    {
    }
    void setValue ( AttributeType value ) { mValue = value; }
    AttributeType& getValue() { return mValue; }
    const AttributeType& getValue() const { return mValue; }
    //
    virtual ByteBuffer convertToBuffer () {
        return ::toBuffer ( mValue );
    }
    virtual unsigned long long convertToBuffer ( ByteBuffer& buffer, unsigned long long index ) {
        return ::toBuffer ( buffer, index, mValue );
    }
    virtual unsigned long long convertFromBuffer ( const ByteBuffer& buffer, unsigned long long index ) {
        return ::fromBuffer ( buffer, index, mValue );
    }
protected:
    AttributeType mValue;
};

////////////////////////////////////////////////////////////
///
///
template < class AttributeType > class ResourceIndexedAttribute :
        virtual public IResourceAttribute
{
public:
    ResourceIndexedAttribute (QString name) :
        IResourceAttribute ( name )
    {
    }
    //
    void addValue ( AttributeType value ) { mValue.append(value); }
    AttributeType& getValue(int index) { return mValue [ index ]; }
    const AttributeType& getValue(int index) const { return mValue.at(index); }
    AttributeType& getValue(QString name) {
        for ( int i = 0 ; i < mValue.size () ; i ++)
            if ( mValue.at(i).getName () == name )
                return mValue.at(i);
    }
    const AttributeType& getValue(QString name) const {
        for ( int i = 0 ; i < mValue.size () ; i ++)
            if ( mValue.at(i).getName () == name )
                return mValue.at(i);
    }
    unsigned int getSize () const { return mValue.size(); }
    const QList < AttributeType >& getContent () const { return mValue; }
    //
    virtual ByteBuffer convertToBuffer () {
        unsigned int length = mValue.size();
        ByteBuffer result = ::toBuffer ( length );
        for ( unsigned int i = 0 ; i < length ; i ++ )
            result.append ( ::toBuffer ( mValue [ i ] ) );
        return result;
    }
    virtual unsigned long long convertToBuffer ( ByteBuffer& buffer, unsigned long long index ) {
        unsigned int length = mValue.size();
        unsigned int lindex = index;
        lindex = ::toBuffer <unsigned int> ( buffer, lindex, length );
        for ( unsigned int i = 0 ; i < length ; i ++ )
            lindex = ::toBuffer ( buffer, lindex, mValue [ i ] );
        return lindex;
    }
    virtual unsigned long long convertFromBuffer ( const ByteBuffer& buffer, unsigned long long index ) {
        unsigned int length;
        unsigned long long lindex = index;
        lindex = ::fromBuffer ( buffer, lindex, length );
        for ( unsigned int i = 0 ; i < length ; i ++ ) {
            AttributeType value;
            lindex = ::fromBuffer ( buffer, lindex, value );
            mValue.append(value);
        }
        return lindex;
    }
protected:
    QList < AttributeType > mValue;
};

////////////////////////////////////////////////////////////
///
///
template < class AttributeType > class ResourceNamedAttribute :
        virtual public IResourceAttribute
{
public:
    ResourceNamedAttribute (QString name) :
        IResourceAttribute ( name )
    {
    }
    //
    void addValue ( QString name, AttributeType value ) { mValue[name] = value; }
    AttributeType& getValue(QString name) {
        return mValue [ name ];
    }
    const AttributeType& getValue(QString name) const {
        return mValue [ name ];
    }
    unsigned int getSize () const { return mValue.size(); }
    QList < QString > keys () const {
        return mValue.keys();
    }
    const QMap < QString, AttributeType >& getContent () const { return mValue; }
    //
    virtual ByteBuffer convertToBuffer () {
        unsigned int length = mValue.size();
        ByteBuffer result = ::toBuffer ( length );
        foreach ( QString key, mValue.keys () ) {
            AttributeType value = mValue [ key ];
            result.append( ::toBuffer ( key ) );
            result.append( ::toBuffer ( value ) );
        }
        return result;
    }
    virtual unsigned long long convertToBuffer ( ByteBuffer& buffer, unsigned long long index ) {
        unsigned int length = mValue.size();
        unsigned int lindex = index;
        lindex = ::toBuffer <unsigned int> ( buffer, lindex, length );
        foreach ( QString key, mValue.keys () ) {
            AttributeType value = mValue [ key ];
            lindex = ::toBuffer ( buffer, lindex, key );
            lindex = ::toBuffer ( buffer, lindex, value );
        }
        return lindex;
    }
    virtual unsigned long long convertFromBuffer ( const ByteBuffer& buffer, unsigned long long index ) {
        unsigned int length;
        unsigned long long lindex = index;
        lindex = ::fromBuffer ( buffer, lindex, length );
        for ( unsigned int i = 0 ; i < length ; i ++ ) {
            QString key;
            AttributeType value;
            lindex = ::fromBuffer ( buffer, lindex, key );
            lindex = ::fromBuffer ( buffer, lindex, value );
            mValue [ key ] = value;
        }
        return lindex;
    }

protected:
    QMap < QString, AttributeType > mValue;
};

////////////////////////////////////////////////////////////
///
///
template <class AttributeType> IResourceAttribute& IResourceAttribute::operator= ( const AttributeType& value ) {
    ResourceAttribute < AttributeType >* tmp = dynamic_cast < ResourceAttribute < AttributeType >* > ( this );
    tmp->setValue ( value );
    return *this;
}

////////////////////////////////////////////////////////////
///
///
template <class AttributeType> void IResourceAttribute::setValue ( AttributeType value ) {
    ResourceAttribute < AttributeType >* tmp = dynamic_cast < ResourceAttribute < AttributeType >* > ( this );
    tmp->setValue ( value );
}

////////////////////////////////////////////////////////////
///
///
template <class AttributeType> AttributeType& IResourceAttribute::getValue () {
    return dynamic_cast < ResourceAttribute < AttributeType >* > ( this )->getValue ();
}
template <class AttributeType> const AttributeType& IResourceAttribute::getValue () const {
    return dynamic_cast < ResourceAttribute < AttributeType >* > ( this )->getValue ();
}

////////////////////////////////////////////////////////////
///
///
template <class AttributeType> void IResourceAttribute::addValue ( AttributeType value ) {
    ResourceIndexedAttribute < AttributeType >* tmp = dynamic_cast < ResourceIndexedAttribute < AttributeType >* > ( this );
    tmp->addValue ( value );
}

////////////////////////////////////////////////////////////
///
///
template <class AttributeType> AttributeType& IResourceAttribute::getValue (int index) {
    return dynamic_cast < ResourceIndexedAttribute < AttributeType >* > ( this )->getValue (index);
}
template <class AttributeType> const AttributeType& IResourceAttribute::getValue (int index) const {
    return dynamic_cast < ResourceIndexedAttribute < AttributeType >* > ( this )->getValue (index);
}

////////////////////////////////////////////////////////////
///
///
template <class AttributeType> void IResourceAttribute::addValue ( QString name, AttributeType value ) {
    ResourceNamedAttribute < AttributeType >* tmp = dynamic_cast < ResourceNamedAttribute < AttributeType >* > ( this );
    tmp->addValue ( name, value );
}

////////////////////////////////////////////////////////////
///
///
template <class AttributeType> AttributeType& IResourceAttribute::getValue (QString name) {
    return dynamic_cast < ResourceNamedAttribute < AttributeType >* > ( this )->getValue (name);
}
template <class AttributeType> const AttributeType& IResourceAttribute::getValue (QString name) const {
    return dynamic_cast < ResourceNamedAttribute < AttributeType >* > ( this )->getValue (name);
}

////////////////////////////////////////////////////////////
///
///
template <class AttributeType> unsigned int IResourceAttribute::getSize () const {
    {
        const ResourceIndexedAttribute < AttributeType >* tmp = dynamic_cast < const ResourceIndexedAttribute < AttributeType >* > ( this );
        if ( tmp != NULL ) return tmp->getSize();
    }
    {
        const ResourceNamedAttribute < AttributeType >* tmp = dynamic_cast < const ResourceNamedAttribute < AttributeType >* > ( this );
        if ( tmp != NULL ) return tmp->getSize();
    }
    return -1;
}

template < class AttributeType > QList < QString > IResourceAttribute::getKeys () const {
    QList < QString > fake;
    {
        const ResourceNamedAttribute < AttributeType >* tmp = dynamic_cast < const ResourceNamedAttribute < AttributeType >* > ( this );
        if ( tmp != NULL ) return tmp->keys();
    }
    return fake;
}

////////////////////////////////////////////////////////////
///
///
typedef QList < IResourceAttribute* > AttributeList;

////////////////////////////////////////////////////////////
///
///
typedef QMap < QString, IResourceAttribute* > AttributeDictionnary;

#endif // IRESOURCEATTRIBUTE_HPP

