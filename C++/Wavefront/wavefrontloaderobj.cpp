#include "wavefrontloaderobj.hpp"

WavefrontLoaderOBJ::WavefrontLoaderOBJ()
{
    HasUUID::InitializeUUID( "WavefrontLoaderOBJ" );
    HasName::Initialize( "WavefrontLoaderOBJ" );
}

WavefrontLoaderOBJ::~WavefrontLoaderOBJ()
{

}

SharedResourceList WavefrontLoaderOBJ::__load ( FileDescriptor filename ) {
    QStringList indices;
    std::cout << "WavefrontLoaderOBJ::load(" << filename.fullFilename.toStdString() << ")" << std::endl;
    SharedResourceList result;
    SharedResourceList materialLib;

    FileTokenizer theFile ( filename );

    Assets::MeshPtr currentObject = ResourceHolder::CreateByName ( "Mesh", filename.fullFilename ).dynamicCast < Assets::Mesh > ();
    //( new Assets::Mesh ( theFile.mDescriptor.fileBasename ) );
    Assets::MaterialGroup* currentMaterial = NULL;

    //
    // PreProcess file to enumerate sub resources
    //
    unsigned int nbv =  0, nbvt = 0, nbvn = 0, nbo = 0, nbf = 0, nbg = 0;
    unsigned int nbgf [ 1024 ];
    for ( int i = 0 ; i < 1024 ; i ++ ) nbgf [i] = 0;
    QString mLine = theFile.readLine ();
    while ( ! theFile.feof() ) {
        if ( mLine.startsWith("vt") ) nbvt ++;
        else if ( mLine.startsWith("vn") ) nbvn ++;
        else if ( mLine.startsWith("v") ) nbv ++;
        else if ( mLine.startsWith("o") ) nbo ++;
        else if ( mLine.startsWith("f") ) {
            QStringList m_tokens = theFile.readTokens ( false );
            unsigned int nbfv = m_tokens.size () - 1;
            nbf += ( nbfv - 3 ) + 1;
            nbgf [ nbg ] += ( nbfv - 3 ) + 1;
        }
        else if ( mLine.startsWith("usemtl") ) nbg ++;
        mLine = theFile.readLine ();
    }

    //
    // Process file
    //
    Vector3d* oVert = new Vector3d [ nbv ];
    Vector2d* oTexv = new Vector2d [ nbvt ];
    Vector3d* oNorm = new Vector3d [ nbvn ];
    Triangle* oTri = new Triangle [ nbf ];
    Triangle* oGrpTri = NULL;
    unsigned int iVert = 0, iTexv = 0, iNorm = 0, iTri = 0, iGrp = 0, iGrpTri = 0;
    theFile.reset();
    QStringList m_tokens = theFile.readTokens ();
    while ( ! theFile.feof() ) {
        m_tokens = theFile.getTokens();
        while ( ! theFile.feof() ) {
            if ( m_tokens.count() > 0 && m_tokens [ 0 ] != "#" ) break;
            m_tokens = theFile.readTokens();
        }
        if ( theFile.feof() ) break;
        if ( m_tokens [ 0 ] == "mtllib" ) {
            QString line = theFile.getLine();
            int pos = line.indexOf ( m_tokens [ 1 ] );
            QString subname = line.right ( line.length() - pos );
            QString fname;
            if ( subname.at(0) == '/' )
                fname = theFile.mDescriptor.fileSubDirectory + subname;
            else
                fname = theFile.mDescriptor.fileSubDirectory + "/" + subname;
            FileDescriptor file ( fname, theFile.mDescriptor.fileDirectory );
            SharedResourceList matAssets = ResourceHolder::Load(file);
            if ( matAssets.size() > 0 ) {
                materialLib = matAssets;
            }
        }
        else if ( m_tokens [ 0 ] == "usemtl" ) {
            iGrp ++; iGrpTri = 0;
            oGrpTri = new Triangle [ nbgf [ iGrp ] ];
            if ( currentMaterial != NULL ) {
                currentObject->add<Assets::MaterialGroup*> ( "MaterialGroups", currentMaterial );
            }
            QString line = theFile.getLine();
            int pos = line.indexOf ( m_tokens [ 1 ] );
            QString subname = line.right ( line.length() - pos );
            Assets::MaterialPtr mat;
            foreach ( SharedResourcePtr tmp, materialLib )
                if ( tmp->get < QString > ( "_RealName" ) == subname ) mat = tmp.dynamicCast < Assets::Material > ();
            currentMaterial = new Assets::MaterialGroup ( currentObject, mat, oGrpTri, nbgf [ iGrp ] );
        }
        else if ( m_tokens [ 0 ] == "o" ) {
            if ( currentMaterial != NULL ) {
                currentObject->add<Assets::MaterialGroup*> ( "MaterialGroups", currentMaterial );
            }
            if ( ( iTri > 0 ) || ( iGrp > 0 ) )
                result.append(currentObject);
            QString line = theFile.getLine();
            int pos = line.indexOf ( m_tokens [ 1 ] );
            QString name = line.right ( line.length() - pos );
            currentObject = ResourceHolder::CreateByName ( "Mesh", name ).dynamicCast < Assets::Mesh > ();
            currentMaterial = NULL;
        }
        else if ( m_tokens [ 0 ] == "v" ) {
            oVert[iVert].x = m_tokens [ 1 ].toFloat();
            oVert[iVert].y = m_tokens [ 2 ].toFloat();
            oVert[iVert].z = m_tokens [ 3 ].toFloat();
            iVert++;
        }
        else if ( m_tokens [ 0 ] == "vn" ) {
            oNorm[iNorm].x = m_tokens [ 1 ].toFloat();
            oNorm[iNorm].y = m_tokens [ 2 ].toFloat();
            oNorm[iNorm].z = m_tokens [ 3 ].toFloat();
            iNorm++;
        }
        else if ( m_tokens [ 0 ] == "vt" ) {
            oTexv[iTexv].x = m_tokens [ 1 ].toFloat();
            oTexv[iTexv].y = m_tokens [ 2 ].toFloat();
            iTexv ++;
        }
        else if ( m_tokens [ 0 ] == "f" ) {
            int mode = 2;
            if ( m_tokens [ 1 ].indexOf ( "//" ) > 0 ) {
                mode = 1;
            } else if ( m_tokens [ 1 ].indexOf ( "/" ) == -1 ) {
                mode = 0;
            } else {
                indices = m_tokens [ 1 ].split ( "/", QString::SkipEmptyParts );
                if ( indices.count () == 2 )
                    mode = 3;
            }
            unsigned int nbv = m_tokens.size () - 1;
            int id [ 3 ];
            int nid [ 3 ];
            int tid [ 3 ];
            for ( int i = 0 ; i < 3 ; i ++ )
                tid [ i ] = nid [ i ] = id [ i ] = -1;
            if ( nbv == 3 ) {
                Triangle t;
                if ( mode >= 2 ) t.m_hasTexcoords = true;
                else if ( mode >= 1 ) t.m_hasNormals = true;
                for ( int i = 0 ; i < 3 ; i ++ ) {
                    indices = m_tokens [ i + 1 ].split ( "/", QString::SkipEmptyParts );
                    t.m_vertexIndices [ i ] = indices [ 0 ].toUInt () - 1;
                    if ( mode >= 2 )
                        t.m_texcoordIndices [ i ] = indices [ 1 ].toUInt () - 1;
                    if ( mode == 1 || mode == 2 )
                        t.m_normalIndices [ i ] = indices [ mode ].toUInt () - 1;
                }
                if ( currentMaterial == NULL ) { oTri[iTri] = t; iTri ++; }
                else { oGrpTri [ iGrpTri ] = t; iGrpTri ++; }
            } else {
                Triangle t;
                if ( mode >= 2 ) t.m_hasTexcoords = true;
                else if ( mode >= 1 ) t.m_hasNormals = true;
                int idp;
                idp = 0; indices = m_tokens [ 1 ].split ( "/", QString::SkipEmptyParts );
                t.m_vertexIndices [ idp ] = indices [ 0 ].toUInt () - 1;
                if ( mode >= 2 )
                    t.m_texcoordIndices [ idp ] = indices [ 1 ].toUInt () - 1;
                if ( mode == 1 || mode == 2 )
                    t.m_normalIndices [ idp ] = indices [ mode ].toUInt () - 1;
                for ( unsigned int j = 1 ; j < nbv - 1 ; j ++ ) {
                    idp = 1; indices = m_tokens [ j + 1 ].split ( "/", QString::SkipEmptyParts );
                    t.m_vertexIndices [ idp ] = indices [ 0 ].toUInt () - 1;
                    if ( mode >= 2 )
                        t.m_texcoordIndices [ idp ] = indices [ 1 ].toUInt () - 1;
                    if ( mode == 1 || mode == 2 )
                        t.m_normalIndices [ idp ] = indices [ mode ].toUInt () - 1;
                    idp = 2; indices = m_tokens [ j + 2 ].split ( "/", QString::SkipEmptyParts );
                    t.m_vertexIndices [ idp ] = indices [ 0 ].toUInt () - 1;
                    if ( mode >= 2 )
                        t.m_texcoordIndices [ idp ] = indices [ 1 ].toUInt () - 1;
                    if ( mode == 1 || mode == 2 )
                        t.m_normalIndices [ idp ] = indices [ mode ].toUInt () - 1;
                    if ( currentMaterial == NULL ) { oTri[iTri] = t; iTri ++; }
                    else { oGrpTri [ iGrpTri ] = t; iGrpTri ++; }
                }
            }
        }
        theFile.readTokens();
    }
    if ( iTri != nbf ) {
        if ( iTri == 0 ) { delete [] oTri; oTri = NULL; }
        else {
            Triangle* tmp = new Triangle [ iTri ];
            memcpy ( tmp, oTri, iTri * sizeof ( Triangle ) );
            delete [] oTri;
            oTri = tmp;
        }
    }
    currentObject->set < unsigned int > ( "#Vertices", iVert );
    currentObject->set < Vector3d* > ( "Vertices", oVert );
    currentObject->set < unsigned int > ( "#TexVertices", iTexv );
    currentObject->set < Vector2d* > ( "TexVertices", oTexv );
    currentObject->set < unsigned int > ( "#Normals", iNorm );
    currentObject->set < Vector3d* > ( "Normals", oNorm );
    currentObject->set < unsigned int > ( "#Faces", iTri );
    currentObject->set < Triangle* > ( "Faces", oTri );
    if ( currentMaterial != NULL ) {
        currentObject->add<Assets::MaterialGroup*> ( "MaterialGroups", currentMaterial );
    }
    if ( ( iTri > 0 ) || ( iGrp > 0 ) )
        result.append(currentObject);
    return result;
}
