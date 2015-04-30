#include "worldcell.hpp"

#include "structures.hpp"

namespace Assets {

WorldCell::WorldCell ()
{
    _createAttributes ();
}

WorldCell::WorldCell (QString name)
{
    HasName::Initialize( name );
    _createAttributes ();
}

WorldCell::~WorldCell ()
{
}

void WorldCell::Usage()
{
}

void WorldCell::_createAttributes ()
{
    QUuid fake;
    Vector3d zeros; zeros.x = zeros.y = zeros.z = 0.0;
    Vector2ui ones; ones.w = ones.h = 1;
    addAttribute < QString > ( "_RealName", "" );
    addAttribute < QUuid > ( "World", fake );
    addAttribute < Vector2ui > ( "Indices", ones );
    addAttribute < Vector3d > ( "Extents", zeros );
    addAttribute < Vector3d > ( "Position", zeros );
    addIndexedAttribute < QUuid > ( "GameObjects" );
}

ByteBuffer WorldCell::_toBuffer() {
    IResourceAttribute* gos = getAttr ( "GameObjects" ); ByteBuffer gosBuf = gos->convertToBuffer();
    ByteBuffer world = ::toBuffer ( get < QUuid > ( "World" ) );
    ByteBuffer realname = ::toBuffer ( get < QString > ( "_RealName" ) );
    unsigned long long totalSize = 2 * sizeof ( Vector3d ) + sizeof ( Vector2ui )
            + realname.getLength()
            + world.getLength()
            + gosBuf.getLength(),
            index = 0;
    ByteBuffer result ( totalSize );
    index = ::toBuffer ( result, index, realname.getData(), realname.getLength() );
    index = ::toBuffer ( result, index, world.getData(), world.getLength() );
    index = ::toBuffer ( result, index, get < Vector3d > ( "Position" ) );
    index = ::toBuffer ( result, index, get < Vector3d > ( "Extents" ) );
    index = ::toBuffer ( result, index, get < Vector2ui > ( "Indices" ) );
    index = ::toBuffer ( result, index, gosBuf.getData(), gosBuf.getLength() );
    return result;
}

unsigned long long WorldCell::_fromBuffer ( const ByteBuffer& buffer, unsigned long long index ) {
    unsigned long long lindex = index;
    IResourceAttribute* attr;
    attr = getAttr("_RealName"); lindex = attr->convertFromBuffer(buffer, lindex);
    attr = getAttr("World"); lindex = attr->convertFromBuffer(buffer, lindex);
    attr = getAttr("Position"); lindex = attr->convertFromBuffer(buffer, lindex);
    attr = getAttr("Extents"); lindex = attr->convertFromBuffer(buffer, lindex);
    attr = getAttr("Indices"); lindex = attr->convertFromBuffer(buffer, lindex);
    attr = getAttr("GameObjects"); lindex = attr->convertFromBuffer(buffer, lindex);
    return lindex;
}

WorldCellPtr WorldCellManager::getByUUID ( QUuid uuid )
{
    return SharedResourceManager < WorldCell >::getByUUID(uuid).dynamicCast < WorldCell > ();
}

WorldCellManager::WorldCellManager() : ISharedResourceManager ( "worldcell/" ), SharedResourceManager < WorldCell > ( "worldcell/" ) {
    HasUUID::InitializeUUID( "worldcell/" );
}

}

