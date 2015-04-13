#include "QImageLoader/qimageloader.hpp"
#include "Wavefront/wavefront.hpp"

int main(int argc, char *argv[])
{
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
