#include "Assets/Image.hpp"

namespace Assets {

Image::Image ()
{
    _createAttributes ();
}

Image::Image (QString name)
{
    HasName::Initialize( name );
    _createAttributes ();
}

Image::~Image ()
{
}

void Image::Usage()
{
    std::cout << getUUID().toString().toStdString() <<
                 " && " << getName ().toStdString() <<
                 " : " << get < unsigned int > ( "Width" ) <<
                 "x" <<  get < unsigned int > ( "Height" ) <<
                 ":" <<  get < unsigned int > ( "Depth" ) <<
                 "/" <<  get < unsigned int > ( "nChannels" ) << std::endl;
}

void Image::_createAttributes ()
{
    addAttribute < unsigned int > ( "Width", 0 );
    addAttribute < unsigned int > ( "Height", 0 );
    addAttribute < unsigned int > ( "Depth", 0 );
    addAttribute < unsigned int > ( "nChannels", 0 );
    addAttribute < unsigned char* > ( "Data", NULL );
}

ByteBuffer Image::_toBuffer () {
    unsigned long long index = 0,
            ww = get < unsigned int > ( "Width" ),
            hh = get < unsigned int > ( "Height" ),
            dd = get < unsigned int > ( "Depth" ),
            nc = get < unsigned int > ( "nChannels" ),
            imageSize = ww * hh * dd / 8;
    unsigned long long totalDataSize = 4 * sizeof ( unsigned int ) + imageSize;
    ByteBuffer buffer ( totalDataSize );
    index = ::toBuffer ( buffer, index, get < unsigned int > ( "Width" ) );
    index = ::toBuffer ( buffer, index, get < unsigned int > ( "Height" ) );
    index = ::toBuffer ( buffer, index, get < unsigned int > ( "Depth" ) );
    index = ::toBuffer ( buffer, index, get < unsigned int > ( "nChannels" ) );
    index = ::toBuffer ( buffer, index, get < unsigned char* > ( "Data" ), imageSize );
    return buffer;
}

unsigned long long Image::_fromBuffer ( const ByteBuffer& buffer, unsigned long long index ) {
    unsigned int tmp;
    unsigned long long lindex = index;
    unsigned char* data;
    lindex = ::fromBuffer ( buffer, lindex, tmp ); set < unsigned int > ( "Width", tmp );
    lindex = ::fromBuffer ( buffer, lindex, tmp ); set < unsigned int > ( "Height", tmp );
    lindex = ::fromBuffer ( buffer, lindex, tmp ); set < unsigned int > ( "Depth", tmp );
    lindex = ::fromBuffer ( buffer, lindex, tmp ); set < unsigned int > ( "nChannels", tmp );
    unsigned long long imageSize = get < unsigned int > ( "Width" ) * get < unsigned int > ( "Height" ) * get < unsigned int > ( "Depth" ) / 8;
    data = new unsigned char [ imageSize ];
    lindex = ::fromBuffer ( buffer, lindex, data, imageSize ); set < unsigned char* > ( "Data", data );
    return lindex;
}

ImageLoader::ImageLoader () {}
ImageLoader::~ImageLoader () {}


ImagePtr ImageManager::getByUUID ( QUuid uuid )
{
    return SharedResourceManager < Image >::getByUUID(uuid).dynamicCast < Image > ();
}

ImageManager::ImageManager() : ISharedResourceManager ( "image/" ), SharedResourceManager < Image > ( "image/" ) {
    HasUUID::InitializeUUID( "image/" );
}

}
