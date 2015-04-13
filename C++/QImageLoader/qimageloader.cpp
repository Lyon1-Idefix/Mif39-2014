#include "qimageloader.hpp"

QImageLoader::QImageLoader()
{

}

QImageLoader::~QImageLoader()
{
    HasUUID::InitializeUUID( "QImageLoader" );
    HasName::Initialize( "QImageLoader" );
}

SharedResourceList QImageLoader::__load ( FileDescriptor filename ) {
    SharedResourceList result;
    Assets::ImagePtr image = ResourceHolder::CreateByName ( "Image", filename.fullFilename ).dynamicCast < Assets::Image > ();
    QImage realImage ( filename.fullFilename );
    int nChannels = realImage.depth()/8;
    unsigned char* data = new unsigned char [realImage.byteCount()];
    memcpy(data,realImage.bits(),realImage.byteCount());
    image->set <unsigned int> ( "Width", realImage.width() );
    image->set <unsigned int> ( "Height", realImage.height() );
    image->set <unsigned int> ( "Depth", realImage.depth() );
    image->set <unsigned int> ( "nChannels", nChannels );
    image->set <unsigned char*> ( "Data", data );
    result.append ( image );
    return result;
}

void __attach(void) __attribute__((constructor));
void __detach(void) __attribute__((destructor));

void __attach(void)
{
    std::cout << "Attaching QImageLoader library" << std::endl;
    std::cout << "Initialize QImage Loaders" << std::endl;
    QMimeDatabase db;
    foreach ( QByteArray ba, QImageReader::supportedMimeTypes() ) {
        if ( ba.size() > 0 ) {
            foreach ( QMimeType type, db.allMimeTypes() )
                if ( type.name() == ba ) {
                    ResourceHolder::RegisterLoader < Assets::Image, QImageLoader > ( type );
                }
        }
    }
}

void __detach(void)
{
    std::cout << "Detaching QImageLoader library" << std::endl;
}
