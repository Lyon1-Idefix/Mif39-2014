#ifndef WAVEFRONTMATERIAL_H
#define WAVEFRONTMATERIAL_H

#include "Assets/assets.hpp"

class WavefrontMaterial :
        virtual public Assets::Material,
        virtual public Tokenized
{
public:
    virtual ~WavefrontMaterial ();
    virtual void Usage ();
protected:
    WavefrontMaterial ();
    WavefrontMaterial (QString name);
    virtual void _createAttributes ();
    virtual bool fillFromTokenizer(FileTokenizer& theFile);
    virtual ByteBuffer _toBuffer ();
    virtual unsigned long long _fromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 );
private:
    QString getAttributeName ( QString token );
    void readAttribute (FileTokenizer& theFile);
    bool fillFromTokenizer (FileTokenizer& theFile, ColorRGB& color);
    bool fillFromTokenizer (FileTokenizer& theFile, Dissolve& dis);
    bool fillFromTokenizer (FileTokenizer& theFile, Texture& tex);
    friend class WavefrontLoaderMTL;
    friend class ResourceDescriptor < WavefrontMaterial >;
};

#endif // WAVEFRONTMATERIAL_H
