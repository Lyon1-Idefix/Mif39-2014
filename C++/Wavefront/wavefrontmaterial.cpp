#include "wavefrontmaterial.hpp"

WavefrontMaterial::WavefrontMaterial ()
{
    _createAttributes ();
}

WavefrontMaterial::WavefrontMaterial (QString name)
{
    HasName::Initialize( name );
    _createAttributes ();
}

WavefrontMaterial::~WavefrontMaterial ()
{
}

void WavefrontMaterial::_createAttributes ()
{
    QUuid fake;
    ColorRGB white = { 1, 1, 1 };
    ColorRGB black = { 0, 0, 0 };
    Dissolve dissolve = { false, 0.0f };
    Texture empty; empty.imageID = fake;
    addAttribute < QString > ( "_RealName", "" );
    addAttribute < ColorRGB > ( "Ambient", black );
    addAttribute < ColorRGB > ( "Diffuse", white );
    addAttribute < ColorRGB > ( "Specular", black );
    addAttribute < ColorRGB > ( "Emissive", black );
    addAttribute < ColorRGB > ( "Transmission", black );
    addAttribute < Dissolve > ( "Dissolve", dissolve );
    addAttribute < float > ( "SpecularExponent", 0.0f );
    addAttribute < float > ( "Sharpness", 0.0f );
    addAttribute < float > ( "OpticalDensity", 0.0f );
    addAttribute < Texture > ( "AmbientMap", empty );
    addAttribute < Texture > ( "DiffuseMap", empty );
    addAttribute < Texture > ( "SpecularMap", empty );
    addAttribute < Texture > ( "SpecularExponentMap", empty );
    addAttribute < Texture > ( "DissolveMap", empty );
    addAttribute < Texture > ( "DecalMap", empty );
    addAttribute < Texture > ( "DisplacementMap", empty );
    addAttribute < Texture > ( "BumpMap", empty );
    addAttribute < Texture > ( "ReflectionMap", empty );
    addAttribute < int > ( "Illumination", 0 );
}

void WavefrontMaterial::Usage()
{
    std::cout << getUUID().toString().toStdString() <<
                 " && " << getName ().toStdString() << std::endl;
}

bool WavefrontMaterial::fillFromTokenizer(FileTokenizer& theFile)
{
    QStringList m_tokens = theFile.getTokens();
    while ( true ) {
        if ( theFile.feof() ) break;
        m_tokens = theFile.readTokens ();
        if ( m_tokens.count() > 0 && m_tokens [ 0 ] == "newmtl" ) break;
        if ( m_tokens.count() > 0 ) {
            readAttribute(theFile);
        }
    }
    return true;
}

ByteBuffer WavefrontMaterial::_toBuffer () {
    Texture tmp;
    ByteBuffer fake = ::toBuffer (tmp);
    unsigned long long index = 0;
    unsigned long long totalSize =
            5 * sizeof ( ColorRGB )
            + 1 * sizeof ( Dissolve )
            + 3 * sizeof ( float )
            + 9 * fake.getLength()
            + 1 * sizeof ( int );
    ByteBuffer result (totalSize);
    index = ::toBuffer ( result, index, get < ColorRGB > ( "Ambient" ) );
    index = ::toBuffer ( result, index, get < ColorRGB > ( "Diffuse" ) );
    index = ::toBuffer ( result, index, get < ColorRGB > ( "Specular" ) );
    index = ::toBuffer ( result, index, get < ColorRGB > ( "Emissive" ) );
    index = ::toBuffer ( result, index, get < ColorRGB > ( "Transmission" ) );
    index = ::toBuffer ( result, index, get < Dissolve > ( "Dissolve" ) );
    index = ::toBuffer ( result, index, get < float > ( "SpecularExponent" ) );
    index = ::toBuffer ( result, index, get < float > ( "Sharpness" ) );
    index = ::toBuffer ( result, index, get < float > ( "OpticalDensity" ) );
    index = ::toBuffer ( result, index, get < Texture > ( "AmbientMap" ) );
    index = ::toBuffer ( result, index, get < Texture > ( "DiffuseMap" ) );
    index = ::toBuffer ( result, index, get < Texture > ( "SpecularMap" ) );
    index = ::toBuffer ( result, index, get < Texture > ( "SpecularExponentMap" ) );
    index = ::toBuffer ( result, index, get < Texture > ( "DissolveMap" ) );
    index = ::toBuffer ( result, index, get < Texture > ( "DecalMap" ) );
    index = ::toBuffer ( result, index, get < Texture > ( "DisplacementMap" ) );
    index = ::toBuffer ( result, index, get < Texture > ( "BumpMap" ) );
    index = ::toBuffer ( result, index, get < Texture > ( "ReflectionMap" ) );
    index = ::toBuffer ( result, index, get < int > ( "Illumination" ) );
    return result;
}

unsigned long long WavefrontMaterial::_fromBuffer ( const ByteBuffer& buffer, unsigned long long index ) {
    ColorRGB color;
    float flt;
    int intv;
    Texture tex;
    Dissolve dis;
    unsigned long long lindex = index;
    lindex = ::fromBuffer ( buffer, lindex, color ); set < ColorRGB > ( "Ambient", color );
    lindex = ::fromBuffer ( buffer, lindex, color ); set < ColorRGB > ( "Diffuse", color );
    lindex = ::fromBuffer ( buffer, lindex, color ); set < ColorRGB > ( "Specular", color );
    lindex = ::fromBuffer ( buffer, lindex, color ); set < ColorRGB > ( "Emissive", color );
    lindex = ::fromBuffer ( buffer, lindex, color ); set < ColorRGB > ( "Transmission", color );
    lindex = ::fromBuffer ( buffer, lindex, dis ); set < Dissolve > ( "Dissolve", dis );
    lindex = ::fromBuffer ( buffer, lindex, flt ); set < float > ( "SpecularExponent", flt );
    lindex = ::fromBuffer ( buffer, lindex, flt ); set < float > ( "Sharpness", flt );
    lindex = ::fromBuffer ( buffer, lindex, flt ); set < float > ( "OpticalDensity", flt );
    lindex = ::fromBuffer ( buffer, lindex, tex ); set < Texture > ( "AmbientMap", tex );
    lindex = ::fromBuffer ( buffer, lindex, tex ); set < Texture > ( "DiffuseMap", tex );
    lindex = ::fromBuffer ( buffer, lindex, tex ); set < Texture > ( "SpecularMap", tex );
    lindex = ::fromBuffer ( buffer, lindex, tex ); set < Texture > ( "SpecularExponentMap", tex );
    lindex = ::fromBuffer ( buffer, lindex, tex ); set < Texture > ( "DissolveMap", tex );
    lindex = ::fromBuffer ( buffer, lindex, tex ); set < Texture > ( "DecalMap", tex );
    lindex = ::fromBuffer ( buffer, lindex, tex ); set < Texture > ( "DisplacementMap", tex );
    lindex = ::fromBuffer ( buffer, lindex, tex ); set < Texture > ( "BumpMap", tex );
    lindex = ::fromBuffer ( buffer, lindex, tex ); set < Texture > ( "ReflectionMap", tex );
    lindex = ::fromBuffer ( buffer, lindex, intv ); set < int > ( "Illumination", intv );
    return lindex;
}

QString WavefrontMaterial::getAttributeName ( QString token ) {
    if ( token == "Ka" ) return "Ambient";
    if ( token == "Kd" ) return "Diffuse";
    if ( token == "Ks" ) return "Specular";
    if ( token == "Ke" ) return "Emissive";
    if ( token == "Tf" ) return "Transmission";
    if ( token == "illum" ) return "Illumination";
    if ( token == "d" ) return "Dissolve";
    if ( token == "Ns" ) return "SpecularExponent";
    if ( token == "sharpness" ) return "Sharpness";
    if ( token == "Ni" ) return "OpticalDensity";
    if ( token == "map_Ka" ) return "AmbientMap";
    if ( token == "map_Kd" ) return "DiffuseMap";
    if ( token == "map_Ks" ) return "SpecularMap";
    if ( token == "map_Ns" ) return "SpecularExponentMap";
    if ( token == "map_d" ) return "DissolveMap";
    if ( token == "decal" ) return "DecalMap";
    if ( token == "disp" ) return "DisplacementMap";
    if ( token == "bump" ) return "BumpMap";
    if ( token == "refl" ) return "ReflectionMap";
    return "";
}

bool WavefrontMaterial::fillFromTokenizer (FileTokenizer& theFile, ColorRGB& color)
{
    QStringList tokens = theFile.getTokens();
    QString line = theFile.getLine();
    QVector3D m_value;
    QString m_spectralFile;
    float m_spectralFactor;
    if ( tokens [ 1 ] == "xyz" ) {
        if ( tokens.count () != 5 ) return false;
        m_value = QVector3D ( tokens [ 2 ].toFloat (), tokens [ 3 ].toFloat (), tokens [ 4 ].toFloat () );
        /***
          needs conversion code
          ***/
    }
    else if ( tokens [ 1 ] == "spectral" ) {
        int start = line.indexOf ( tokens [ 1 ] ) + tokens [ 1 ].length ();
        m_spectralFile = line.right ( line.length () - start );
        m_spectralFile = m_spectralFile.left ( m_spectralFile.length () - tokens [ tokens.count () - 1 ].length () );
        m_spectralFile = m_spectralFile.trimmed ();
        m_spectralFactor = tokens [ tokens.count () - 1 ].toFloat ();
        /***
          needs conversion code
          ***/
    }
    else {
        if ( tokens.count () != 4 ) return false;
        color.r = tokens [ 1 ].toFloat ();
        color.g = tokens [ 2 ].toFloat ();
        color.b = tokens [ 3 ].toFloat ();
    }
    return true;
}

bool WavefrontMaterial::fillFromTokenizer (FileTokenizer& theFile, Dissolve& dis)
{
    QStringList tokens = theFile.getTokens();
    if ( tokens [ 1 ] == "-halo" ) {
        dis.factor = tokens [ 2 ].toFloat ();
        dis.halo = true;
    }
    else {
        dis.factor = tokens [ 1 ].toFloat ();
        dis.halo = false;
    }
    return true;
}

bool WavefrontMaterial::fillFromTokenizer (FileTokenizer& theFile, Texture& tex)
{
    QStringList tokens = theFile.getTokens();
    QString line = theFile.getLine();
    QUuid m_image;
    bool m_blendu = true;
    bool m_blendv = true;
    bool m_cc = false;
    bool m_clamp = false;
    float m_base = 0;
    float m_gain = 1;
    Vector3d m_position = { 0, 0, 0 };
    Vector3d m_scale = { 1, 1, 1 };
    Vector3d m_turbulence = { 0, 0, 0 };
    int m_texres = 1;
    float m_bumpmult = 1;
    float m_boost = 0;
    TextureChannel m_channel = CHAN_L;
    //
    int i = 1;
    while ( i< tokens.count () ) {
        if ( i >= tokens.count () ) return false;
        if ( tokens [ i ] == "-blendu" ) {
            m_blendu =  ( tokens [ i + 1 ] == "on" );
            i += 2;
        }
        else if ( tokens [ i ] == "-blendv" ) {
            m_blendv =  ( tokens [ i + 1 ] == "on" );
            i += 2;
        }
        else if ( tokens [ i ] == "-cc" ) {
            m_cc =  ( tokens [ i + 1 ] == "on" );
            i += 2;
        }
        else if ( tokens [ i ] == "-clamp" ) {
            m_clamp =  ( tokens [ i + 1 ] == "on" );
            i += 2;
        }
        else if ( tokens [ i ] == "-mm" ) {
            m_base =  tokens [ i + 1 ].toFloat ();
            m_gain =  tokens [ i + 2 ].toFloat ();
            i += 3;
        }
        else if ( tokens [ i ] == "-texres" ) {
            m_texres = tokens [ i + 1 ].toInt ();
            i += 2;
        }
        else if ( tokens [ i ] == "-bm" ) {
            m_bumpmult =  tokens [ i + 1 ].toFloat ();
            i += 2;
        }
        else if ( tokens [ i ] == "-boost" ) {
            m_boost =  tokens [ i + 1 ].toFloat ();
            i += 2;
        }
        else if ( tokens [ i ] == "-imfchan" ) {
            if ( tokens [ i + 1 ] == "r" ) m_channel = CHAN_R;
            else if ( tokens [ i + 1 ] == "g" ) m_channel = CHAN_G;
            else if ( tokens [ i + 1 ] == "b" ) m_channel = CHAN_B;
            else if ( tokens [ i + 1 ] == "m" ) m_channel = CHAN_M;
            else if ( tokens [ i + 1 ] == "l" ) m_channel = CHAN_L;
            else if ( tokens [ i + 1 ] == "z" ) m_channel = CHAN_Z;
            else return false;
            i += 2;
        }
        else if ( tokens [ i ] == "-o" ) {
            bool ok;
            m_position.x = tokens [ i + 1 ].toFloat ();
            ok = false; m_position.y = tokens [ i + 2 ].toFloat ( & ok );
            if ( ok ) {
                ok = false; m_position.z = tokens [ i + 3 ].toFloat ( & ok );
                if ( ok ) {
                    i += 4;
                }
                else {
                    m_position.z = 0;
                    i += 3;
                }
            }
            else {
                m_position.y = 0;
                m_position.z = 0;
                i += 2;
            }
        }
        else if ( tokens [ i ] == "-s" ) {
            bool ok;
            m_scale.x = tokens [ i + 1 ].toFloat ();
            ok = false; m_scale.y = tokens [ i + 2 ].toFloat ( & ok );
            if ( ok ) {
                ok = false; m_scale.z = tokens [ i + 3 ].toFloat ( & ok );
                if ( ok ) {
                    i += 4;
                }
                else {
                    m_scale.z = 0;
                    i += 3;
                }
            }
            else {
                m_scale.y = 0;
                m_scale.z = 0;
                i += 2;
            }
        }
        else if ( tokens [ i ] == "-t" ) {
            bool ok;
            m_turbulence.x = tokens [ i + 1 ].toFloat ();
            ok = false; m_turbulence.y = tokens [ i + 2 ].toFloat ( & ok );
            if ( ok ) {
                ok = false; m_turbulence.z = tokens [ i + 3 ].toFloat ( & ok );
                if ( ok ) {
                    i += 4;
                }
                else {
                    m_turbulence.z = 0;
                    i += 3;
                }
            }
            else {
                m_turbulence.y = 0;
                m_turbulence.z = 0;
                i += 2;
            }
        }
        else break;
    }
    if ( i >= tokens.count () ) return false;
    int pos = line.indexOf ( tokens [ i ] );
    QString subname = line.right ( line.length() - pos );
    QString fname;
    if ( subname.at(0) == '/' )
        fname = theFile.mDescriptor.fileDirectory + subname;
    else
        fname = theFile.mDescriptor.fileDirectory + "/" + subname;
    SharedResourceList imgAssets = ResourceHolder::Load(fname);
    if ( imgAssets.size() == 1 ) {
        m_image = imgAssets[0]->getUUID ();
    }
    //
    tex.imageID = m_image;
    tex.blendU = m_blendu;
    tex.blendV = m_blendv;
    tex.CC = m_cc;
    tex.Clamp = m_clamp;
    tex.Base = m_base;
    tex.Gain = m_gain;
    tex.BumpMult = m_bumpmult;
    tex.Boost = m_boost;
    tex.TexRes = m_texres;
    tex.Position = m_position;
    tex.Scale = m_scale;
    tex.Turbulence = m_turbulence;
    tex.Channel = m_channel;
    //
    return true;
}

void WavefrontMaterial::readAttribute (FileTokenizer& theFile) {
    QStringList tokens = theFile.getTokens();
    QString token = tokens [ 0 ];
    QString name = getAttributeName ( token );
    if ( name == "" ) return;
    if ( ( token == "Ka" ) || ( token == "Kd" ) || ( token == "Ks" ) || ( token == "Ke" ) || ( token == "Tf" ) ) {
        ColorRGB color;
        fillFromTokenizer(theFile, color);
        set (name, color );
    }
    else if ( token == "illum" ) {
        int val = strtol ( tokens [ 1 ].toStdString().c_str(), NULL, 10 );
        set (name, val );
    }
    else if ( token == "d" ) {
        Dissolve val;
        fillFromTokenizer(theFile, val);
        set (name, val);
    }
    else if ( ( token == "Ns" ) || ( token == "sharpness" ) || ( token == "Ni" ) ) {
        float flt = strtod ( tokens [ 1 ].toStdString().c_str(), NULL );
        set (name, flt );
    }
    else {
        Texture tex;
        fillFromTokenizer(theFile, tex);
        set (name, tex);
    }
}
