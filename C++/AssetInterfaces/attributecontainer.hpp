#ifndef ASSETCONTAINER_H
#define ASSETCONTAINER_H

#include "AssetInterfaces/internals.hpp"
#include "AssetInterfaces/iresourceattribute.hpp"

class AttributeContainer
{
public:
    IResourceAttribute& operator[] ( QString name );
    IResourceAttribute* getAttr ( QString name ) { return mAttributes [ name ]; }
    const IResourceAttribute* getAttr ( QString name ) const { return mAttributes [ name ]; }
    //
    template < class AttributeType > void set ( QString name, AttributeType value ) {
        ( mAttributes [ name ] )->setValue < AttributeType > ( value );
    }
    template < class AttributeType > AttributeType& get ( QString name ) {
        return ( mAttributes [ name ] )->getValue < AttributeType > ();
    }
    template < class AttributeType > const AttributeType& get ( QString name ) const {
        return ( mAttributes [ name ] )->getValue < const AttributeType > ();
    }
    //
    template < class AttributeType > void add ( QString name, AttributeType value ) {
        ( mAttributes [ name ] )->addValue < AttributeType > ( value );
    }
    template < class AttributeType > AttributeType& get ( QString name, int index ) {
        return ( mAttributes [ name ] )->getValue < AttributeType > (index);
    }
    template < class AttributeType > const AttributeType& get ( QString name, int index ) const {
        return ( mAttributes [ name ] )->getValue < const AttributeType > (index);
    }
    //
    template < class AttributeType > void add ( QString name, QString vname, AttributeType value ) {
        ( mAttributes [ name ] )->addValue < AttributeType > ( vname, value );
    }
    template < class AttributeType > AttributeType& get ( QString name, QString vname ) {
        return ( mAttributes [ name ] )->getValue < AttributeType > (vname);
    }
    template < class AttributeType > const AttributeType& get ( QString name, QString vname ) const {
        return ( mAttributes [ name ] )->getValue < const AttributeType > (vname);
    }
    //
    template < class AttributeType > unsigned int getSize ( QString name ) const {
        return ( mAttributes [ name ] )->getSize < AttributeType > ();
    }
    //
    template < class AttributeType > QList < QString > getKeys ( QString name ) const {
        return ( mAttributes [ name ] )->getKeys < AttributeType > ();
    }
protected:
    AttributeContainer ();
    template < class AttributeType > void addAttribute ( QString name, const AttributeType& value ) {
        mAttributes [ name ] = new ResourceAttribute < AttributeType > ( name, value );
    }
    template < class AttributeType > void addIndexedAttribute ( QString name ) {
        mAttributes [ name ] = new ResourceIndexedAttribute < AttributeType > ( name );
    }
    template < class AttributeType > void addNamedAttribute ( QString name ) {
        mAttributes [ name ] = new ResourceNamedAttribute < AttributeType > ( name );
    }
    virtual void _createAttributes () = 0;
    AttributeDictionnary mAttributes;
};


#endif // ASSETCONTAINER_H
