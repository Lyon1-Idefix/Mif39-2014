#include "Assets/Mesh.hpp"

namespace Assets {

MaterialGroup::MaterialGroup ()
{
    _createAttributes();
}

MaterialGroup::MaterialGroup ( MeshPtr mesh, MaterialPtr material, Triangle* faces, unsigned int nfaces )
{
    HasName::Initialize( material->getName() );
    _createAttributes();
    set < QUuid > ( "Material", material->getUUID() );
    set < QUuid > ( "Mesh", mesh->getUUID() );
    set < unsigned int > ( "#Faces", nfaces );
    set < Triangle* > ( "Faces", faces );
}

void MaterialGroup::_createAttributes ()
{
    QUuid fake;
    addAttribute < QUuid > ( "Material", fake );
    addAttribute < QUuid > ( "Mesh", fake );
    addAttribute < unsigned int > ( "#Faces", 0 );
    addAttribute < Triangle* > ( "Faces", NULL );
}

ByteBuffer MaterialGroup::_toBuffer ()
{
    unsigned long long index = 0, nfaces = get < unsigned int > ( "#Faces" );
    ByteBuffer mat = ::toBuffer ( get < QUuid > ( "Material" ) );
    ByteBuffer mesh = ::toBuffer ( get < QUuid > ( "Mesh" ) );
    unsigned long long totalDataSize = mat.getLength() + mesh.getLength() +
            sizeof ( unsigned int ) +
            nfaces * sizeof ( Triangle );
    ByteBuffer buffer ( totalDataSize );
    index = ::toBuffer ( buffer, index, mat.getData(), mat.getLength () );
    index = ::toBuffer ( buffer, index, mesh.getData(), mesh.getLength () );
    index = ::toBuffer ( buffer, index, get < unsigned int > ( "#Faces" ) );
    index = ::toBuffer ( buffer, index, get < Triangle* > ( "Faces" ), get < unsigned int > ( "#Faces" ) );
    return buffer;
}

unsigned long long MaterialGroup::_fromBuffer ( const ByteBuffer& buffer, unsigned long long index )
{
    QUuid tmp;
    unsigned long long lindex = index;
    lindex = ::fromBuffer ( buffer, lindex, tmp ); set < QUuid > ( "Material", tmp );
    lindex = ::fromBuffer ( buffer, lindex, tmp ); set < QUuid > ( "Mesh", tmp );
    {
        unsigned int count;
        lindex = ::fromBuffer ( buffer, lindex, count ); set < unsigned int > ( "#Faces", count );
        Triangle* tri = new Triangle [ count ];
        lindex = ::fromBuffer ( buffer, lindex, tri, count ); set < Triangle* > ( "Faces", tri );
    }
    return lindex;
}

Mesh::Mesh ()
{
    _createAttributes ();
}

Mesh::Mesh (QString name)
{
    HasName::Initialize( name );
    _createAttributes ();
}

Mesh::~Mesh ()
{
}

void Mesh::Usage()
{
    std::cout << getUUID().toString().toStdString() <<
                 " && " << getName ().toStdString() <<
                 " #v: " << get < unsigned int > ( "#Vertices" ) <<
                 " #t: " << get < unsigned int > ( "#TexVertices" ) <<
                 " #n: " << get < unsigned int > ( "#Normals" ) <<
                 " #f: " << get < unsigned int > ( "#Faces" ) <<
                 " #g: " << getSize < MaterialGroup* > ( "MaterialGroups" ) << std::endl;
}

void Mesh::_createAttributes ()
{
    addAttribute < unsigned int > ( "#Vertices", 0 );
    addAttribute < unsigned int > ( "#TexVertices", 0 );
    addAttribute < unsigned int > ( "#Normals", 0 );
    addAttribute < unsigned int > ( "#Faces", 0 );
    addAttribute < Vector3d* > ( "Vertices", NULL );
    addAttribute < Vector2d* > ( "TexVertices", NULL );
    addAttribute < Vector3d* > ( "Normals", NULL );
    addAttribute < Triangle* > ( "Faces", NULL );
    addIndexedAttribute < MaterialGroup* > ( "MaterialGroups" );
}

ByteBuffer Mesh::_toBuffer() {
    ByteBuffer uuid = ::toBuffer ( getUUID() );
    ByteBuffer name = ::toBuffer ( getName () );

    unsigned long long totalSize = 5 * sizeof ( unsigned int ) +
            get < unsigned int > ( "#Vertices" ) * sizeof ( Vector3d ) +
            get < unsigned int > ( "#TexVertices" ) * sizeof ( Vector2d ) +
            get < unsigned int > ( "#Normals" ) * sizeof ( Vector3d )
            + get < unsigned int > ( "#Faces" ) * sizeof ( Triangle ),
            index = 0;
    ByteBuffer result ( totalSize );
    index = ::toBuffer ( result, index, get < unsigned int > ( "#Vertices" ) );
    index = ::toBuffer ( result, index, get < Vector3d* > ( "Vertices" ), get < unsigned int > ( "#Vertices" ) );
    index = ::toBuffer ( result, index, get < unsigned int > ( "#TexVertices" ) );
    index = ::toBuffer ( result, index, get < Vector2d* > ( "TexVertices" ), get < unsigned int > ( "#TexVertices" ) );
    index = ::toBuffer ( result, index, get < unsigned int > ( "#Normals" ) );
    index = ::toBuffer ( result, index, get < Vector3d* > ( "Normals" ), get < unsigned int > ( "#Normals" ) );
    /**/
    index = ::toBuffer ( result, index, get < unsigned int > ( "#Faces" ) );
    index = ::toBuffer ( result, index, get < Triangle* > ( "Faces" ), get < unsigned int > ( "#Faces" ) );
    /**/
    index = ::toBuffer ( result, index, getSize < MaterialGroup* > ( "MaterialGroups" ) );
    for ( unsigned int i = 0 ; i < getSize < MaterialGroup* > ( "MaterialGroups" ) ; i ++ ) {
        MaterialGroup* grp = get < MaterialGroup* > ( "MaterialGroups", i );
        ByteBuffer grpBuf = grp->convertToBuffer ();
        result.append(grpBuf);
    }
    return result;
}

unsigned long long Mesh::_fromBuffer ( const ByteBuffer& buffer, unsigned long long index ) {
    unsigned long long lindex = index;
    {
        unsigned int count;
        lindex = ::fromBuffer ( buffer, lindex, count ); set < unsigned int > ( "#Vertices", count );
        Vector3d* tri = new Vector3d [ count ];
        lindex = ::fromBuffer ( buffer, lindex, tri, count ); set < Vector3d* > ( "Vertices", tri );
    }
    {
        unsigned int count;
        lindex = ::fromBuffer ( buffer, lindex, count ); set < unsigned int > ( "#TexVertices", count );
        Vector2d* tri = new Vector2d [ count ];
        lindex = ::fromBuffer ( buffer, lindex, tri, count ); set < Vector2d* > ( "TexVertices", tri );
    }
    {
        unsigned int count;
        lindex = ::fromBuffer ( buffer, lindex, count ); set < unsigned int > ( "#Normals", count );
        Vector3d* tri = new Vector3d [ count ];
        lindex = ::fromBuffer ( buffer, lindex, tri, count ); set < Vector3d* > ( "Normals", tri );
    }
    /**/
    {
        unsigned int count;
        lindex = ::fromBuffer ( buffer, lindex, count ); set < unsigned int > ( "#Faces", count );
        Triangle* tri = new Triangle [ count ];
        lindex = ::fromBuffer ( buffer, lindex, tri, count ); set < Triangle* > ( "Faces", tri );
    }
    /**/
    {
        unsigned int count;
        lindex = ::fromBuffer ( buffer, lindex, count );
        for ( int i = 0 ; i < count ; i ++ ) {
            MaterialGroup* mat = new MaterialGroup;
            lindex = mat->convertFromBuffer ( buffer, lindex );
            add<MaterialGroup*> ( "MaterialGroups", mat );
        }
    }
    return lindex;
}

MeshLoader::MeshLoader () {}
MeshLoader::~MeshLoader () {}


MeshPtr MeshManager::getByUUID ( QUuid uuid )
{
    return SharedResourceManager < Mesh >::getByUUID(uuid).dynamicCast < Mesh > ();
}

MeshManager::MeshManager() : ISharedResourceManager ( "geometry/" ), SharedResourceManager < Mesh > ( "geometry/" ) {
    HasUUID::InitializeUUID( "geometry/" );
}

}

