#include "world.hpp"
#include "worldcell.hpp"
#include "gameobject.hpp"
#include "gameentity.hpp"

#include "structures.hpp"

namespace Assets {

SpawnPoint::SpawnPoint ()
{
    _createAttributes();
}

SpawnPoint::SpawnPoint ( WorldPtr world, QString name )
{
    HasName::Initialize( name );
    _createAttributes();
    set < QUuid > ( "World", world->getUUID() );
}

void SpawnPoint::_createAttributes ()
{
    QUuid fake;
    Vector3d zeros;
    addAttribute < QString > ( "_RealName", "" );
    addAttribute < QUuid > ( "World", fake );
    addAttribute < Vector3d > ( "Location", zeros );
    addAttribute < float > ( "Size", 1.0f );
    addIndexedAttribute < QString > ( "Semantics" );
}

ByteBuffer SpawnPoint::_toBuffer ()
{
    ByteBuffer realname = ::toBuffer ( get < QString > ( "_RealName" ) );
    ByteBuffer world = ::toBuffer ( get < QUuid > ( "World" ) );
    unsigned long long index = 0;
    unsigned long long totalDataSize = sizeof ( Vector3d ) + sizeof ( float ) + sizeof ( unsigned int ) + realname.getLength() + world.getLength();
    ByteBuffer result ( totalDataSize );
    index = ::toBuffer ( result, index, realname.getData(), realname.getLength() );
    index = ::toBuffer ( result, index, world.getData(), world.getLength() );
    index = ::toBuffer ( result, index, get < Vector3d > ( "Location" ) );
    index = ::toBuffer ( result, index, get < float > ( "Size" ) );
    index = ::toBuffer <unsigned int> ( result, index, getSize < QString > ( "Semantics" ) );
    for ( uint i = 0 ; i < getSize < QString > ( "Semantics" ) ; i ++ ) {
        ByteBuffer semantic = ::toBuffer ( get < QString > ( "Semantics", i ) );
        result.append ( semantic );
    }
    return result;
}

unsigned long long SpawnPoint::_fromBuffer ( const ByteBuffer& buffer, unsigned long long index )
{
    unsigned long long lindex = index;
    IResourceAttribute* attr;
    attr = getAttr("_RealName"); lindex = attr->convertFromBuffer(buffer, lindex);
    attr = getAttr("World"); lindex = attr->convertFromBuffer(buffer, lindex);
    attr = getAttr("Location"); lindex = attr->convertFromBuffer(buffer,lindex);
    attr = getAttr("Size"); lindex = attr->convertFromBuffer(buffer,lindex);
    attr = getAttr("Semantics"); lindex = attr->convertFromBuffer(buffer,lindex);
    return lindex;
}

World::World ()
{
    _createAttributes ();
}

World::World (QString name)
{
    HasName::Initialize( name );
    _createAttributes ();
}

World::~World ()
{
}

void World::Usage()
{
    std::cout << getUUID().toString().toStdString() << std::endl;
    std::cout << "\t" << getName().toStdString() << std::endl;
    std::cout << "\t" << get < QString > ( "_RealName" ).toStdString() << std::endl;
    std::cout << "\t" << get < Vector3d > ( "Extents" ) << std::endl;
    std::cout << "\t" << get < Vector2ui > ( "Subdivision" ) << std::endl;
    std::cout << "\t" << getSize < SpawnPoint* > ( "SpawnPoints" ) << std::endl;
    std::cout << "\t" << getSize < QString > ( "Semantics" ) << std::endl;
    std::cout << "\t" << getSize < QUuid > ( "Cells" ) << std::endl;
}

void World::_createAttributes ()
{
    Vector3d zeros; zeros.x = zeros.y = zeros.z = 0.0;
    Vector2ui ones; ones.w = ones.h = 1;
    addAttribute < QString > ( "_RealName", "" );
    addAttribute < Vector3d > ( "Extents", zeros );
    addAttribute < Vector2ui > ( "Subdivision", ones );
    addIndexedAttribute < SpawnPoint* > ( "SpawnPoints" );
    addIndexedAttribute < QString > ( "Semantics" );
    addIndexedAttribute < QUuid > ( "Cells" );
}

ByteBuffer World::_toBuffer() {
    ByteBuffer realname = ::toBuffer ( get < QString > ( "_RealName" ) );
    ByteBuffer spawnsBuf ( 4 );
    unsigned int tindex = 0;
    tindex = ::toBuffer ( spawnsBuf, tindex, getSize < SpawnPoint* > ( "SpawnPoints" ) );
    for ( unsigned int i = 0 ; i < getSize < SpawnPoint* > ( "SpawnPoints" ) ; i ++ ) {
        SpawnPoint* grp = get < SpawnPoint* > ( "SpawnPoints", i );
        ByteBuffer grpBuf = grp->convertToBuffer ();
        spawnsBuf.append(grpBuf);
    }
    IResourceAttribute* semantics = getAttr ( "Semantics" ); ByteBuffer semanticsBuf = semantics->convertToBuffer();
    IResourceAttribute* cells = getAttr ( "Cells" ); ByteBuffer cellsBuf = cells->convertToBuffer();
    unsigned long long totalSize = sizeof ( Vector3d ) + sizeof ( Vector2ui )
            + realname.getLength()
            + spawnsBuf.getLength()
            + semanticsBuf.getLength()
            + cellsBuf.getLength(),
            index = 0;
    ByteBuffer result ( totalSize );
    index = ::toBuffer ( result, index, realname.getData(), realname.getLength() );
    index = ::toBuffer ( result, index, get < Vector3d > ( "Extents" ) );
    index = ::toBuffer ( result, index, get < Vector2ui > ( "Subdivision" ) );
    index = ::toBuffer ( result, index, spawnsBuf.getData(), spawnsBuf.getLength() );
    index = ::toBuffer ( result, index, semanticsBuf.getData(), semanticsBuf.getLength() );
    index = ::toBuffer ( result, index, cellsBuf.getData(), cellsBuf.getLength() );
    return result;
}

unsigned long long World::_fromBuffer ( const ByteBuffer& buffer, unsigned long long index ) {
    unsigned long long lindex = index;
    IResourceAttribute* attr;
    attr = getAttr ( "_RealName" ); lindex = attr->convertFromBuffer(buffer,lindex);
    attr = getAttr ( "Extents" ); lindex = attr->convertFromBuffer(buffer,lindex);
    attr = getAttr ( "Subdivision" ); lindex = attr->convertFromBuffer(buffer,lindex);
    //attr = getAttr ( "SpawnPoints" ); lindex = attr->convertFromBuffer(buffer,lindex);
    {
        unsigned int count;
        lindex = ::fromBuffer ( buffer, lindex, count );
        for ( int i = 0 ; i < count ; i ++ ) {
            SpawnPoint* mat = new SpawnPoint;
            lindex = mat->convertFromBuffer ( buffer, lindex );
            add<SpawnPoint*> ( "SpawnPoints", mat );
        }
    }
    attr = getAttr ( "Semantics" ); lindex = attr->convertFromBuffer(buffer,lindex);
    attr = getAttr ( "Cells" ); lindex = attr->convertFromBuffer(buffer,lindex);
    return lindex;
}

WorldLoader::WorldLoader () {}
WorldLoader::~WorldLoader () {}

SharedResourceList WorldLoader::__load ( FileDescriptor filename ) {
    //std::cout << "WorldLoader::load(" << filename.fullFilename.toStdString() << ")" << std::endl;
    SharedResourceList result;

    WorldPtr currentWorld ( NULL );
    WorldCellPtr currentCell ( NULL );
    GameObjectPtr currentObject ( NULL );
    SpawnPoint* currentSpawn = NULL;
    SharedResourceList entityLib;

    FileTokenizer theFile ( filename );
    QStringList m_tokens = theFile.readTokens ();
    while ( ! theFile.feof() ) {
        m_tokens = theFile.getTokens();
        while ( ! theFile.feof() ) {
            if ( m_tokens.count() > 0 && m_tokens [ 0 ] != "#" ) break;
            m_tokens = theFile.readTokens();
        }
        if ( theFile.feof() ) break;
        if ( m_tokens[0] == "world" ) {
            QString line = theFile.getLine();
            int pos = line.indexOf ( m_tokens [ 1 ] );
            QString name = line.right ( line.length() - pos );
            currentWorld = ResourceHolder::CreateByName ( "World", name ).dynamicCast < Assets::World > ();
            currentWorld->set < QString > ( "_RealName", name );
            result.append(currentWorld);
        } else if ( m_tokens[0] == "spawnpoint" ) {
            QString line = theFile.getLine();
            int pos = line.indexOf ( m_tokens [ 1 ] );
            QString name = line.right ( line.length() - pos );
            currentSpawn = new Assets::SpawnPoint ( currentWorld, name );
            currentSpawn->set < QString > ( "_RealName", name );
            currentSpawn->set < QUuid > ( "World", currentWorld->getUUID() );
            currentWorld->add < SpawnPoint* > ( "SpawnPoints", currentSpawn );
        } else if ( m_tokens[0] == "cell" ) {
            QString line = theFile.getLine();
            int pos = line.indexOf ( m_tokens [ 1 ] );
            QString name = line.right ( line.length() - pos );
            currentCell = ResourceHolder::CreateByName ( "WorldCell", name ).dynamicCast < Assets::WorldCell > ();
            currentCell->set < QString > ( "_RealName", name );
            currentCell->set < QUuid > ( "World", currentWorld->getUUID() );
            currentWorld->add < QUuid > ( "Cells", currentCell->getUUID() );
        } else if ( m_tokens[0] == "gameelement" ) {
            QString line = theFile.getLine();
            int pos = line.indexOf ( m_tokens [ 1 ] );
            QString name = line.right ( line.length() - pos );
            currentObject = ResourceHolder::CreateByName ( "GameObject", name ).dynamicCast < Assets::GameObject > ();
            currentObject->set < QString > ( "_RealName", name );
            currentObject->set < QUuid > ( "Cell", currentCell->getUUID() );
            currentCell->add < QUuid > ( "GameObjects", currentObject->getUUID() );
        } else if ( m_tokens[0] == "entities" ) {
            QString line = theFile.getLine();
            int pos = line.indexOf ( m_tokens [ 1 ] );
            QString subname = line.right ( line.length() - pos );
            QString fname;
            fname = theFile.mDescriptor.fileDirectory + "/" + theFile.mDescriptor.fileSubDirectory + "/" + subname;
            FileDescriptor file ( fname, theFile.mDescriptor.fileDirectory );
            SharedResourceList matAssets = ResourceHolder::Load(file);
            if ( matAssets.size() > 0 ) {
                entityLib = matAssets;
            }
        } else if ( m_tokens[0] == "extents" ) {
            Vector3d v;
            v.x = m_tokens [ 1 ].toFloat();
            v.y = m_tokens [ 2 ].toFloat();
            v.z = m_tokens [ 3 ].toFloat();
            if ( currentCell.isNull() )
                currentWorld->set < Vector3d > ( "Extents", v );
            else
                currentCell->set < Vector3d > ( "Extents", v );
        } else if ( m_tokens[0] == "subdivision" ) {
            Vector2ui v;
            v.w = m_tokens [ 1 ].toUInt();
            v.h = m_tokens [ 2 ].toUInt();
            currentWorld->set < Vector2ui > ( "Subdivision", v );
        } else if ( m_tokens[0] == "semantic" ) {
            QString line = theFile.getLine();
            int pos = line.indexOf ( m_tokens [ 1 ] );
            QString name = line.right ( line.length() - pos );
            if ( currentObject.isNull() )
                currentWorld->add < QString > ( "Semantics", name );
            else
                currentObject->add < QString > ( "Semantics", name );
        } else if ( m_tokens[0] == "size" ) {
            float v;
            v = m_tokens [ 1 ].toFloat();
            currentSpawn->set < float > ( "Size", v );
        } else if ( m_tokens[0] == "position" ) {
            Vector3d v;
            v.x = m_tokens [ 1 ].toFloat();
            v.y = m_tokens [ 2 ].toFloat();
            v.z = m_tokens [ 3 ].toFloat();
            if ( currentObject.isNull() )
                currentSpawn->set < Vector3d > ( "Location", v );
            else
                currentObject->set < Vector3d > ( "Position", v );
        } else if ( m_tokens[0] == "center" ) {
            Vector3d v;
            v.x = m_tokens [ 1 ].toFloat();
            v.y = m_tokens [ 2 ].toFloat();
            v.z = m_tokens [ 3 ].toFloat();
            currentCell->set < Vector3d > ( "Position", v );
        } else if ( m_tokens[0] == "indices" ) {
            Vector2ui v;
            v.w = m_tokens [ 1 ].toUInt();
            v.h = m_tokens [ 2 ].toUInt();
            currentCell->set < Vector2ui > ( "Indices", v );
        } else if ( m_tokens[0] == "rotation" ) {
            Vector4d v;
            v.x = m_tokens [ 1 ].toFloat();
            v.y = m_tokens [ 2 ].toFloat();
            v.z = m_tokens [ 3 ].toFloat();
            v.w = m_tokens [ 4 ].toFloat();
            currentObject->set < Vector4d > ( "Rotation", v );
        } else if ( m_tokens[0] == "scale" ) {
            Vector3d v;
            v.x = m_tokens [ 1 ].toFloat();
            v.y = m_tokens [ 2 ].toFloat();
            v.z = m_tokens [ 3 ].toFloat();
            currentObject->set < Vector3d > ( "Scale", v );
        } else if ( m_tokens[0] == "instanceof" ) {
            QString line = theFile.getLine();
            int pos = line.indexOf ( m_tokens [ 1 ] );
            QString subname = line.right ( line.length() - pos );
            Assets::GameEntityPtr entity;
            foreach ( SharedResourcePtr tmp, entityLib )
                if ( tmp->get < QString > ( "_RealName" ) == subname ) entity = tmp.dynamicCast < Assets::GameEntity > ();
            if ( ! entity.isNull() )
                currentObject->set < QUuid > ( "Entity", entity->getUUID() );
        }
        theFile.readTokens();
    }
    return result;
}


WorldPtr WorldManager::getByUUID ( QUuid uuid )
{
    return SharedResourceManager < World >::getByUUID(uuid).dynamicCast < World > ();
}

WorldManager::WorldManager() : ISharedResourceManager ( "world/" ), SharedResourceManager < World > ( "world/" ) {
    HasUUID::InitializeUUID( "world/" );
}

}

