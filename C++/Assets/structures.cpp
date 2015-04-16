#include "structures.hpp"

template <> unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const Texture& value )
{
    unsigned long long lindex = index;
    lindex = ::toBuffer ( buffer, lindex, value.imageID );
    lindex = ::toBuffer ( buffer, lindex, value.blendU );
    lindex = ::toBuffer ( buffer, lindex, value.blendV );
    lindex = ::toBuffer ( buffer, lindex, value.CC );
    lindex = ::toBuffer ( buffer, lindex, value.Clamp );
    lindex = ::toBuffer ( buffer, lindex, value.Base );
    lindex = ::toBuffer ( buffer, lindex, value.Gain );
    lindex = ::toBuffer ( buffer, lindex, value.BumpMult );
    lindex = ::toBuffer ( buffer, lindex, value.Boost );
    lindex = ::toBuffer ( buffer, lindex, value.TexRes );
    lindex = ::toBuffer ( buffer, lindex, value.Position );
    lindex = ::toBuffer ( buffer, lindex, value.Scale );
    lindex = ::toBuffer ( buffer, lindex, value.Turbulence );
    lindex = ::toBuffer ( buffer, lindex, value.Channel );
    return lindex;
}

template <> unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, Texture& value )
{
    unsigned long long lindex = index;
    lindex = ::fromBuffer ( buffer, lindex, value.imageID );
    lindex = ::fromBuffer ( buffer, lindex, value.blendU );
    lindex = ::fromBuffer ( buffer, lindex, value.blendV );
    lindex = ::fromBuffer ( buffer, lindex, value.CC );
    lindex = ::fromBuffer ( buffer, lindex, value.Clamp );
    lindex = ::fromBuffer ( buffer, lindex, value.Base );
    lindex = ::fromBuffer ( buffer, lindex, value.Gain );
    lindex = ::fromBuffer ( buffer, lindex, value.BumpMult );
    lindex = ::fromBuffer ( buffer, lindex, value.Boost );
    lindex = ::fromBuffer ( buffer, lindex, value.TexRes );
    lindex = ::fromBuffer ( buffer, lindex, value.Position );
    lindex = ::fromBuffer ( buffer, lindex, value.Scale );
    lindex = ::fromBuffer ( buffer, lindex, value.Turbulence );
    lindex = ::fromBuffer ( buffer, lindex, value.Channel );
    return lindex;
}

template <> ByteBuffer toBuffer ( const Texture& value )
{
    ByteBuffer uuid = ::toBuffer ( value.imageID);
    unsigned long long totalSize = uuid.getLength() +
            4 * sizeof ( bool ) + 4 * sizeof ( float ) + sizeof ( int ) + 3 * sizeof ( Vector3d ) + sizeof ( TextureChannel ),
            lindex = 0;
    ByteBuffer buffer ( totalSize );
    lindex = ::toBuffer ( buffer, lindex, uuid.getData(), uuid.getLength() );
    lindex = ::toBuffer ( buffer, lindex, value.blendU );
    lindex = ::toBuffer ( buffer, lindex, value.blendV );
    lindex = ::toBuffer ( buffer, lindex, value.CC );
    lindex = ::toBuffer ( buffer, lindex, value.Clamp );
    lindex = ::toBuffer ( buffer, lindex, value.Base );
    lindex = ::toBuffer ( buffer, lindex, value.Gain );
    lindex = ::toBuffer ( buffer, lindex, value.BumpMult );
    lindex = ::toBuffer ( buffer, lindex, value.Boost );
    lindex = ::toBuffer ( buffer, lindex, value.TexRes );
    lindex = ::toBuffer ( buffer, lindex, value.Position );
    lindex = ::toBuffer ( buffer, lindex, value.Scale );
    lindex = ::toBuffer ( buffer, lindex, value.Turbulence );
    lindex = ::toBuffer ( buffer, lindex, value.Channel );
    return buffer;
}


template <> unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const Dissolve& value ) {
    unsigned long long lindex = index;
    lindex = ::toBuffer ( buffer, lindex, value.halo );
    lindex = ::toBuffer ( buffer, lindex, value.factor );
    return lindex;
}

template <> unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, Dissolve& value ) {
    unsigned long long lindex = index;
    lindex = ::fromBuffer ( buffer, lindex, value.halo );
    lindex = ::fromBuffer ( buffer, lindex, value.factor );
    return lindex;
}

template <> ByteBuffer toBuffer ( const Dissolve& value )
{
    unsigned long long totalSize = sizeof ( bool ) + sizeof ( float ),
            lindex = 0;
    ByteBuffer buffer ( totalSize );
    lindex = ::toBuffer ( buffer, lindex, value.halo );
    lindex = ::toBuffer ( buffer, lindex, value.factor );
    return buffer;
}

template <> unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const Triangle& value ) {
    unsigned long long lindex = index;
    lindex = ::toBuffer ( buffer, lindex, value.m_hasNormals );
    lindex = ::toBuffer ( buffer, lindex, value.m_hasTexcoords );
    lindex = ::toBuffer ( buffer, lindex, value.m_vertexIndices, 3 );
    lindex = ::toBuffer ( buffer, lindex, value.m_normalIndices, 3 );
    lindex = ::toBuffer ( buffer, lindex, value.m_texcoordIndices, 3 );
    return lindex;
}

template <> unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, Triangle& value ) {
    unsigned long long lindex = index;
    lindex = ::fromBuffer ( buffer, lindex, value.m_hasNormals );
    lindex = ::fromBuffer ( buffer, lindex, value.m_hasTexcoords );
    lindex = ::fromBuffer ( buffer, lindex, value.m_vertexIndices, 3 );
    lindex = ::fromBuffer ( buffer, lindex, value.m_normalIndices, 3 );
    lindex = ::fromBuffer ( buffer, lindex, value.m_texcoordIndices, 3 );
    return lindex;
}

template <> ByteBuffer toBuffer ( const Triangle& value )
{
    unsigned long long totalSize = 2 * sizeof ( bool ) + 3 * 3 * sizeof ( int ),
            lindex = 0;
    ByteBuffer buffer ( totalSize );
    lindex = ::toBuffer ( buffer, lindex, value.m_hasNormals );
    lindex = ::toBuffer ( buffer, lindex, value.m_hasTexcoords );
    lindex = ::toBuffer ( buffer, lindex, value.m_vertexIndices, 3 );
    lindex = ::toBuffer ( buffer, lindex, value.m_normalIndices, 3 );
    lindex = ::toBuffer ( buffer, lindex, value.m_texcoordIndices, 3 );
    return buffer;
}
