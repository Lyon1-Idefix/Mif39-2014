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
    ResourceHolder::Load(file);
    ResourceHolder::Usage();
    foreach ( QUuid id, ResourceHolder::AllKeys() ) {
        SharedResourcePtr res = ResourceHolder::GetByUUID(id);
        res->Usage();
        ByteBuffer buffer = ResourceHolder::ToBuffer( res );
        unsigned long long index = 0;
        SharedResourcePtr ptr = ResourceHolder::FromBuffer(buffer,index);
        ptr->Usage();
    }
    return 0;
}
