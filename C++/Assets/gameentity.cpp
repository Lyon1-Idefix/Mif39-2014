#include "gameentity.hpp"

#include "structures.hpp"

namespace Assets {

GameEntityElement::GameEntityElement ()
{
    _createAttributes();
}

GameEntityElement::GameEntityElement ( GameEntityPtr entity, QString name )
{
    HasName::Initialize( name );
    _createAttributes();
    set < QUuid > ( "Entity", entity->getUUID() );
}

void GameEntityElement::_createAttributes ()
{
    Vector4d ident; ident.x = ident.y = ident.z = 0.0; ident.w = 1.0;
    Vector3d zeros; zeros.x = zeros.y = zeros.z = 0.0;
    Vector3d ones; ones.x = ones.y = ones.z = 1.0;
    QUuid fake;
    addAttribute < QUuid > ( "Entity", fake );
    addAttribute < Vector3d > ( "Position", zeros );
    addAttribute < Vector4d > ( "Rotation", ident );
    addAttribute < Vector3d > ( "Scale", ones );
    addIndexedAttribute < QString > ( "Semantics" );
    addIndexedAttribute < QUuid > ( "Resources" );
}

ByteBuffer GameEntityElement::_toBuffer ()
{
    IResourceAttribute* semantics = getAttr ( "Semantics" ); ByteBuffer semanticsBuf = semantics->convertToBuffer();
    IResourceAttribute* resources = getAttr ( "Resources" ); ByteBuffer resourcesBuf = resources->convertToBuffer();
    ByteBuffer entity = ::toBuffer ( get < QUuid > ( "Entity" ) );
    unsigned long long totalSize = 2 * sizeof ( Vector3d ) + sizeof ( Vector4d )
            + entity.getLength()
            + semanticsBuf.getLength()
            + resourcesBuf.getLength(),
            index = 0;
    ByteBuffer result ( totalSize );
    index = ::toBuffer ( result, index, entity.getData(), entity.getLength() );
    index = ::toBuffer ( result, index, get < Vector3d > ( "Position" ) );
    index = ::toBuffer ( result, index, get < Vector4d > ( "Rotation" ) );
    index = ::toBuffer ( result, index, get < Vector3d > ( "Scale" ) );
    index = ::toBuffer ( result, index, semanticsBuf.getData(), semanticsBuf.getLength() );
    index = ::toBuffer ( result, index, resourcesBuf.getData(), resourcesBuf.getLength() );
    return result;
}

unsigned long long GameEntityElement::_fromBuffer ( const ByteBuffer& buffer, unsigned long long index )
{
    unsigned long long lindex = index;
    IResourceAttribute* attr;
    attr = getAttr("Entity"); lindex = attr->convertFromBuffer(buffer,lindex);
    attr = getAttr("Position"); lindex = attr->convertFromBuffer(buffer,lindex);
    attr = getAttr("Rotation"); lindex = attr->convertFromBuffer(buffer,lindex);
    attr = getAttr("Scale"); lindex = attr->convertFromBuffer(buffer,lindex);
    attr = getAttr("Semantics"); lindex = attr->convertFromBuffer(buffer,lindex);
    attr = getAttr("Resources"); lindex = attr->convertFromBuffer(buffer,lindex);
    return lindex;
}

GameEntity::GameEntity ()
{
    _createAttributes ();
}

GameEntity::GameEntity (QString name)
{
    HasName::Initialize( name );
    _createAttributes ();
}

GameEntity::~GameEntity ()
{
}

void GameEntity::Usage()
{
}

void GameEntity::_createAttributes ()
{
    addAttribute < QString > ( "_RealName", "" );
    addIndexedAttribute < QString > ( "Semantics" );
    addIndexedAttribute < GameEntityElement* > ( "Elements" );
}

ByteBuffer GameEntity::_toBuffer() {
    ByteBuffer elementsBuf ( 4 );
    unsigned int tindex = 0;
    tindex = ::toBuffer ( elementsBuf, tindex, getSize < GameEntityElement* > ( "Elements" ) );
    for ( unsigned int i = 0 ; i < getSize < GameEntityElement* > ( "Elements" ) ; i ++ ) {
        GameEntityElement* grp = get < GameEntityElement* > ( "Elements", i );
        ByteBuffer grpBuf = grp->convertToBuffer ();
        elementsBuf.append(grpBuf);
    }
    IResourceAttribute* semantics = getAttr ( "Semantics" ); ByteBuffer semanticsBuf = semantics->convertToBuffer();
    ByteBuffer realname = ::toBuffer ( get < QString > ( "_RealName" ) );
    unsigned long long totalSize = semanticsBuf.getLength()
            + realname.getLength()
            + elementsBuf.getLength(),
            index = 0;
    ByteBuffer result ( totalSize );
    index = ::toBuffer ( result, index, realname.getData(), realname.getLength() );
    index = ::toBuffer ( result, index, semanticsBuf.getData(), semanticsBuf.getLength() );
    index = ::toBuffer ( result, index, elementsBuf.getData(), elementsBuf.getLength() );
    return result;
}

unsigned long long GameEntity::_fromBuffer ( const ByteBuffer& buffer, unsigned long long index ) {
    unsigned long long lindex = index;
    IResourceAttribute* attr;
    attr = getAttr ( "_RealName" ); lindex = attr->convertFromBuffer(buffer,lindex);
    attr = getAttr ( "Semantics" ); lindex = attr->convertFromBuffer(buffer,lindex);
    //attr = getAttr ( "Elements" ); lindex = attr->convertFromBuffer(buffer,lindex);
    {
        unsigned int count;
        lindex = ::fromBuffer ( buffer, lindex, count );
        for ( int i = 0 ; i < count ; i ++ ) {
            GameEntityElement* mat = new GameEntityElement;
            lindex = mat->convertFromBuffer ( buffer, lindex );
            add<GameEntityElement*> ( "Elements", mat );
        }
    }
    return lindex;
}


GameEntityLoader::GameEntityLoader () {}
GameEntityLoader::~GameEntityLoader () {}

SharedResourceList GameEntityLoader::__load ( FileDescriptor filename ) {
    //std::cout << "GameEntityLoader::load(" << filename.fullFilename.toStdString() << ")" << std::endl;
    SharedResourceList result;

    GameEntityPtr currentEntity;
    GameEntityElement* currentElement = NULL;

    FileTokenizer theFile ( filename );
    QStringList m_tokens = theFile.readTokens ();
    while ( ! theFile.feof() ) {
        m_tokens = theFile.getTokens();
        while ( ! theFile.feof() ) {
            if ( m_tokens.count() > 0 && m_tokens [ 0 ] != "#" ) break;
            m_tokens = theFile.readTokens();
        }
        if ( theFile.feof() ) break;
        if ( m_tokens[0] == "gameentity" ) {
            QString line = theFile.getLine();
            int pos = line.indexOf ( m_tokens [ 1 ] );
            QString name = line.right ( line.length() - pos );
            currentEntity = ResourceHolder::CreateByName ( "GameEntity", name ).dynamicCast < Assets::GameEntity > ();
            currentEntity->set < QString > ( "_RealName", name );
            result.append(currentEntity);
            currentElement = NULL;
        } else if ( m_tokens[0] == "element" ) {
            QString line = theFile.getLine();
            int pos = line.indexOf ( m_tokens [ 1 ] );
            QString name = line.right ( line.length() - pos );
            currentElement = new GameEntityElement ( currentEntity, name );
            currentEntity->add<GameEntityElement*> ( "Elements", currentElement );
        } else if ( m_tokens[0] == "position" ) {
            Vector3d v;
            v.x = m_tokens [ 1 ].toFloat();
            v.y = m_tokens [ 2 ].toFloat();
            v.z = m_tokens [ 3 ].toFloat();
            currentElement->set < Vector3d > ( "Position", v );
        } else if ( m_tokens[0] == "rotation" ) {
            Vector4d v;
            v.x = m_tokens [ 1 ].toFloat();
            v.y = m_tokens [ 2 ].toFloat();
            v.z = m_tokens [ 3 ].toFloat();
            v.w = m_tokens [ 4 ].toFloat();
            currentElement->set < Vector4d > ( "Rotation", v );
        } else if ( m_tokens[0] == "scale" ) {
            Vector3d v;
            v.x = m_tokens [ 1 ].toFloat();
            v.y = m_tokens [ 2 ].toFloat();
            v.z = m_tokens [ 3 ].toFloat();
            currentElement->set < Vector3d > ( "Scale", v );
        } else if ( m_tokens[0] == "resource" ) {
            QString line = theFile.getLine();
            int pos = line.indexOf ( m_tokens [ 1 ] );
            QString subname = line.right ( line.length() - pos );
            QString fname;
            fname = theFile.mDescriptor.fileDirectory + "/" + theFile.mDescriptor.fileSubDirectory + "/" + subname;
            FileDescriptor file ( fname, theFile.mDescriptor.fileDirectory );
            SharedResourceList assets = ResourceHolder::Load(file);
            for ( int i = 0 ; i < assets.size() ; i ++ ) {
                currentElement->add < QUuid > ( "Resources", assets [ i ]->getUUID() );
            }
        } else if ( m_tokens[0] == "subsemantic" ) {
            QString line = theFile.getLine();
            int pos = line.indexOf ( m_tokens [ 1 ] );
            QString name = line.right ( line.length() - pos );
            currentElement->add < QString > ( "Semantics", name );
        } else if ( m_tokens[0] == "semantic" ) {
            QString line = theFile.getLine();
            int pos = line.indexOf ( m_tokens [ 1 ] );
            QString name = line.right ( line.length() - pos );
            currentEntity->add < QString > ( "Semantics", name );
        }
        theFile.readTokens();
    }
    return result;
}

GameEntityPtr GameEntityManager::getByUUID ( QUuid uuid )
{
    return SharedResourceManager < GameEntity >::getByUUID(uuid).dynamicCast < GameEntity > ();
}

GameEntityManager::GameEntityManager() : ISharedResourceManager ( "entity/" ), SharedResourceManager < GameEntity > ( "entity/" ) {
    HasUUID::InitializeUUID( "entity/" );
}

}

