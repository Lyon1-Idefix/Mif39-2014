#include "gameobject.hpp"

#include "structures.hpp"

namespace Assets {

GameObject::GameObject ()
{
    _createAttributes ();
}

GameObject::GameObject (QString name)
{
    HasName::Initialize( name );
    _createAttributes ();
}

GameObject::~GameObject ()
{
}

void GameObject::Usage()
{
}

void GameObject::_createAttributes ()
{
    QUuid fake;
    Vector4d ident; ident.x = ident.y = ident.z = 0.0; ident.w = 1.0;
    Vector3d zeros; zeros.x = zeros.y = zeros.z = 0.0;
    Vector3d ones; ones.x = ones.y = ones.z = 1;
    addAttribute < QString > ( "_RealName", "" );
    addAttribute < QUuid > ( "Cell", fake );
    addAttribute < Vector3d > ( "Position", zeros );
    addAttribute < Vector4d > ( "Rotation", ident );
    addAttribute < Vector3d > ( "Scale", ones );
    addAttribute < QUuid > ( "Entity", fake );
    addIndexedAttribute < QString > ( "Semantics" );
}

ByteBuffer GameObject::_toBuffer() {
    IResourceAttribute* semantics = getAttr ( "Semantics" ); ByteBuffer semanticsBuf = semantics->convertToBuffer();
    ByteBuffer realname = ::toBuffer ( get < QString > ( "_RealName" ) );
    ByteBuffer entity = ::toBuffer ( get < QUuid > ( "Entity" ) );
    ByteBuffer cell = ::toBuffer ( get < QUuid > ( "Cell" ) );
    unsigned long long totalSize = 2 * sizeof ( Vector3d ) + sizeof ( Vector4d )
            + realname.getLength()
            + entity.getLength()
            + semanticsBuf.getLength()
            + cell.getLength(),
            index = 0;
    ByteBuffer result ( totalSize );
    index = ::toBuffer ( result, index, realname.getData(), realname.getLength() );
    index = ::toBuffer ( result, index, cell.getData(), cell.getLength() );
    index = ::toBuffer ( result, index, get < Vector3d > ( "Position" ) );
    index = ::toBuffer ( result, index, get < Vector4d > ( "Rotation" ) );
    index = ::toBuffer ( result, index, get < Vector3d > ( "Scale" ) );
    index = ::toBuffer ( result, index, semanticsBuf.getData(), semanticsBuf.getLength() );
    index = ::toBuffer ( result, index, entity.getData(), entity.getLength() );
    return result;
}

unsigned long long GameObject::_fromBuffer ( const ByteBuffer& buffer, unsigned long long index ) {
    unsigned long long lindex = index;
    IResourceAttribute* attr;
    attr = getAttr("_RealName"); lindex = attr->convertFromBuffer(buffer,lindex);
    attr = getAttr("Cell"); lindex = attr->convertFromBuffer(buffer,lindex);
    attr = getAttr("Position"); lindex = attr->convertFromBuffer(buffer,lindex);
    attr = getAttr("Rotation"); lindex = attr->convertFromBuffer(buffer,lindex);
    attr = getAttr("Scale"); lindex = attr->convertFromBuffer(buffer,lindex);
    attr = getAttr("Semantics"); lindex = attr->convertFromBuffer(buffer,lindex);
    attr = getAttr("Entity"); lindex = attr->convertFromBuffer(buffer,lindex);
    return lindex;
}

GameObjectPtr GameObjectManager::getByUUID ( QUuid uuid )
{
    return SharedResourceManager < GameObject >::getByUUID(uuid).dynamicCast < GameObject > ();
}

GameObjectManager::GameObjectManager() : ISharedResourceManager ( "GameObject/" ), SharedResourceManager < GameObject > ( "GameObject/" ) {
    HasUUID::InitializeUUID( "GameObject/" );
}

}

