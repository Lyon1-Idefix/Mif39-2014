#ifndef QIMAGELOADER_H
#define QIMAGELOADER_H

#include "Assets/assets.hpp"

class QImageLoader :
        virtual public Assets::ImageLoader,
        virtual public Singleton < QImageLoader >
{
public:
    virtual ~QImageLoader ();
protected:
    QImageLoader ();
    virtual SharedResourceList __load ( FileDescriptor filename );
    friend class Singleton < QImageLoader >;
private:
};

#endif // QIMAGELOADER_H
