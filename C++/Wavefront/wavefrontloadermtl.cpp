#include "wavefrontloadermtl.hpp"

WavefrontLoaderMTL::WavefrontLoaderMTL()
{
    HasUUID::InitializeUUID( "WavefrontLoaderMTL" );
    HasName::Initialize( "WavefrontLoaderMTL" );
}

WavefrontLoaderMTL::~WavefrontLoaderMTL()
{

}

SharedResourceList WavefrontLoaderMTL::__load ( FileDescriptor filename ) {
    std::cout << "WavefrontLoaderMTL::load(" << filename.fullFilename.toStdString() << ")" << std::endl;
    SharedResourceList result;
    FileTokenizer theFile ( filename );
    QStringList m_tokens = theFile.readTokens ();
    while ( ! theFile.feof() ) {
        while ( ! theFile.feof() ) {
            if ( m_tokens.count() > 0 && m_tokens [ 0 ] == "newmtl" ) break;
            m_tokens = theFile.readTokens ();
        }
        if ( theFile.feof() ) break;
        m_tokens = theFile.getTokens();
        if ( m_tokens [ 0 ] == "newmtl" ) {
            QString line = theFile.getLine();
            int pos = line.indexOf ( m_tokens [ 1 ] );
            QString subname = line.right ( line.length() - pos );
            Assets::MaterialPtr material = ResourceHolder::CreateByName ( "WavefrontMaterial", subname ).dynamicCast < Assets::Material > (); //new WavefrontMaterial ( subname );
            //Assets::MaterialPtr m ( material );
            material.dynamicCast < WavefrontMaterial > ()->fillFromTokenizer(theFile);
            result.append ( material );
        }
    }
    return result;
}
