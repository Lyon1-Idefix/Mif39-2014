#include "Assets/Light.hpp"

namespace Assets {

Light::Light ()
{
    _createAttributes ();
}

Light::Light (QString name)
{
    HasName::Initialize( name );
    _createAttributes ();
}

Light::~Light ()
{
}

void Light::Usage()
{
}

void Light::_createAttributes ()
{
    Vector3d white = { 1.0f, 1.0f, 1.0f };
    addAttribute < QString > ( "_RealName", "" );
    addAttribute < Light::Type > ( "Type", Light::Point );
    addAttribute < float > ( "Intensity", 1.0f );
    addAttribute < Vector3d > ( "Color", white );
}

ByteBuffer Light::_toBuffer() {
    ByteBuffer realname = ::toBuffer ( get < QString > ( "_RealName" ) );
    ByteBuffer type = ::toBuffer ( get < Light::Type > ( "Type" ) );
    ByteBuffer intensity = ::toBuffer ( get < float > ( "Intensity" ) );
    ByteBuffer color = ::toBuffer ( get < Vector3d > ( "Color" ) );

    unsigned long long totalSize = realname.getLength() + type.getLength() + intensity.getLength() + color.getLength(),
            index = 0;
    ByteBuffer result ( totalSize );
    index = ::toBuffer ( result, index, realname.getData(), realname.getLength() );
    index = ::toBuffer ( result, index, type.getData(), type.getLength() );
    index = ::toBuffer ( result, index, intensity.getData(), intensity.getLength() );
    index = ::toBuffer ( result, index, color.getData(), color.getLength() );
    return result;
}

unsigned long long Light::_fromBuffer ( const ByteBuffer& buffer, unsigned long long index ) {
    unsigned long long lindex = index;
    IResourceAttribute* attr;
    attr = getAttr("_RealName"); lindex = attr->convertFromBuffer(buffer,lindex);
    attr = getAttr("Type"); lindex = attr->convertFromBuffer(buffer,lindex);
    attr = getAttr("Intensity"); lindex = attr->convertFromBuffer(buffer,lindex);
    attr = getAttr("Color"); lindex = attr->convertFromBuffer(buffer,lindex);
    return lindex;
}

LightLoader::LightLoader () {}
LightLoader::~LightLoader () {}

SharedResourceList LightLoader::__load ( FileDescriptor filename ) {
    //std::cout << "LightLoader::load(" << filename.fullFilename.toStdString() << ")" << std::endl;
    SharedResourceList result;

    LightPtr currentLight;

    FileTokenizer theFile ( filename );
    QStringList m_tokens = theFile.readTokens ();
    while ( ! theFile.feof() ) {
        m_tokens = theFile.getTokens();
        while ( ! theFile.feof() ) {
            if ( m_tokens.count() > 0 && m_tokens [ 0 ] != "#" ) break;
            m_tokens = theFile.readTokens();
        }
        if ( theFile.feof() ) break;
        if ( m_tokens[0] == "light" ) {
            QString line = theFile.getLine();
            int pos = line.indexOf ( m_tokens [ 1 ] );
            QString name = line.right ( line.length() - pos );
            currentLight = ResourceHolder::CreateByName ( "Light", name ).dynamicCast < Assets::Light > ();
            currentLight->set < QString > ( "_RealName", name );
            result.append(currentLight);
        } else if ( m_tokens[0] == "type" ) {
            if ( m_tokens [ 1 ] == "Point" )
                currentLight->set < Light::Type > ( "Type", Light::Point );
            else if ( m_tokens [ 1 ] == "Direction" )
                currentLight->set < Light::Type > ( "Type", Light::Direction );
            else if ( m_tokens [ 1 ] == "Surface" )
                currentLight->set < Light::Type > ( "Type", Light::Surface );
        } else if ( m_tokens[0] == "intensity" ) {
            currentLight->set < float > ( "Intensity", m_tokens [ 1 ].toFloat() );
        } else if ( m_tokens[0] == "color" ) {
            Vector3d color;
            color.x = m_tokens [ 1 ].toFloat();
            color.y = m_tokens [ 2 ].toFloat();
            color.z = m_tokens [ 3 ].toFloat();
            currentLight->set < Vector3d > ( "Color", color );
        }
        theFile.readTokens();
    }
    return result;
}

LightPtr LightManager::getByUUID ( QUuid uuid )
{
    return SharedResourceManager < Light >::getByUUID(uuid).dynamicCast < Light > ();
}

LightManager::LightManager() : ISharedResourceManager ( "light/" ), SharedResourceManager < Light > ( "light/" ) {
    HasUUID::InitializeUUID( "light/" );
}

}

