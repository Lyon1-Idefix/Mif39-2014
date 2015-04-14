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
    template <class AttributeType> void setValue ( const AttributeType& value );
    template <class AttributeType> AttributeType& getValue ();
    template <class AttributeType> const AttributeType& getValue () const;
    //
    template <class AttributeType> void addValue ( const AttributeType& value );
    template <class AttributeType> AttributeType& getValue (int index);
    template <class AttributeType> const AttributeType& getValue (int index) const;
    //
    template <class AttributeType> void addValue ( QString name, const AttributeType& value );
    template <class AttributeType> AttributeType& getValue (QString name);
    template <class AttributeType> const AttributeType& getValue (QString name) const;
    //
    template <class AttributeType> unsigned int getSize () const;
    template < class AttributeType > QList < QString > getKeys () const;
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
    void setValue ( const AttributeType& value ) { mValue = value; }
    AttributeType& getValue() { return mValue; }
    const AttributeType& getValue() const { return mValue; }
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
    void addValue ( const AttributeType& value ) { mValue.append(value); }
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
    void addValue ( QString name, const AttributeType& value ) { mValue[name] = value; }
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
template <class AttributeType> void IResourceAttribute::setValue ( const AttributeType& value ) {
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
template <class AttributeType> void IResourceAttribute::addValue ( const AttributeType& value ) {
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
template <class AttributeType> void IResourceAttribute::addValue ( QString name, const AttributeType& value ) {
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

