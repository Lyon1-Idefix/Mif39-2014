#include "QImageLoader/qimageloader.hpp"
#include "Wavefront/wavefront.hpp"

#include <iostream>

extern void __attach(void);
extern void __attachInterfaces(void);
extern void __attachGenerics(void);
extern void __attachAssets(void);
extern void __attachQImage(void);
extern void __attachWavefront(void);

int main(int argc, char *argv[])
{
    /*
    __attach();
    __attachInterfaces();
    __attachGenerics();
    __attachAssets();*/
    __attachQImage();
    __attachWavefront();
    FileDescriptor file ( argv[1]);
    QList < SharedResourcePtr > ress = ResourceHolder::Load(file);
    /*
     * ByteBuffer tmp = ResourceHolder::ToBuffer(ress[0]);
    for ( unsigned int i = 0 ; i < tmp.getLength() ; i ++ ) {
        printf ( "%02x:", tmp.getData() [ i ] );
        if ( ( i % 16 ) == 0 ) printf ( "\n" );
    }*/

    foreach ( QUuid id, ResourceHolder::AllKeys() ) {
        SharedResourcePtr res = ResourceHolder::GetByUUID(id);
        res->Usage();
    }
    return 0;
}
