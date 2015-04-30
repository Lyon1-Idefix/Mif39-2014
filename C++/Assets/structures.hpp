#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include "Utility/utility.hpp"

typedef struct {
    float r, g, b;
} ColorRGB;
inline std::ostream& operator<< ( std::ostream& out, ColorRGB v ) {
    out << LOGVALUEOF(v.r) << "," << LOGVALUEOF(v.g) << "," << LOGVALUEOF(v.b);
    return out;
}

typedef struct {
    float x, y;
} Vector2d;
inline std::ostream& operator<< ( std::ostream& out, Vector2d v ) {
    out << LOGVALUEOF(v.x) << "," << LOGVALUEOF(v.y);
    return out;
}

typedef struct {
    uint w, h;
} Vector2ui;
inline std::ostream& operator<< ( std::ostream& out, Vector2ui v ) {
    out << LOGVALUEOF(v.w) << "," << LOGVALUEOF(v.h);
    return out;
}

typedef struct {
    float x, y, z;
} Vector3d;
inline std::ostream& operator<< ( std::ostream& out, Vector3d v ) {
    out << LOGVALUEOF(v.x) << "," << LOGVALUEOF(v.y) << "," << LOGVALUEOF(v.z);
    return out;
}

typedef struct {
    float x, y, z, w;
} Vector4d;
inline std::ostream& operator<< ( std::ostream& out, Vector4d v ) {
    out << LOGVALUEOF(v.x) << "," << LOGVALUEOF(v.y) << "," << LOGVALUEOF(v.z) << "," << LOGVALUEOF(v.w);
    return out;
}

typedef struct {
    bool m_hasNormals;
    bool m_hasTexcoords;
    int m_vertexIndices [ 3 ];
    int m_normalIndices [ 3 ];
    int m_texcoordIndices [ 3 ];
} Triangle;

typedef struct {
    bool halo;
    float factor;
} Dissolve;

typedef enum { CHAN_R, CHAN_G, CHAN_B, CHAN_M, CHAN_L, CHAN_Z } TextureChannel;

typedef struct {
    QUuid imageID;
    bool blendU, blendV, CC, Clamp;
    float Base, Gain, BumpMult, Boost;
    int TexRes;
    Vector3d Position, Scale, Turbulence;
    TextureChannel Channel;
} Texture;

template <> unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const Texture& value );
template <> unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, Texture& value );
template <> ByteBuffer toBuffer ( const Texture& value );

template <> unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const Dissolve& value );
template <> unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, Dissolve& value );
template <> ByteBuffer toBuffer ( const Dissolve& value );

template <> unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const Triangle& value );
template <> unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, Triangle& value );
template <> ByteBuffer toBuffer ( const Triangle& value );

#endif // STRUCTURES_HPP

