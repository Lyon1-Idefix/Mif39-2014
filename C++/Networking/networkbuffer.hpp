#ifndef NETWORKBUFFER_H
#define NETWORKBUFFER_H

#include "Utility/utility.hpp"

class BufferingPolicyOptions
{
public:
};

class SingleBufferingOptions :
        public BufferingPolicyOptions
{
public:
};

class FiFoBufferingOptions :
        public BufferingPolicyOptions
{
public:
};

class LiFoBufferingOptions :
        public BufferingPolicyOptions
{
public:
};

class BufferingPolicy
{
public:
    virtual ~BufferingPolicy();
    virtual bool add ( ByteBuffer* buffer ) = 0;
    virtual void get ( ByteBuffer*& buffer ) = 0;
    virtual void get ( QList < ByteBuffer* >& buffer ) = 0;
    virtual bool available () = 0;
    virtual void clear () = 0;
protected:
    BufferingPolicy();
    pthread_mutex_t mBufferLock;
};

class SingleBuffering :
        virtual public BufferingPolicy
{
public:
    SingleBuffering ();
    virtual ~SingleBuffering ();
    virtual bool add ( ByteBuffer* buffer );
    virtual void get ( ByteBuffer*& buffer );
    virtual void get ( QList < ByteBuffer* >& buffer );
    virtual bool available ();
    virtual void clear ();
protected:
    ByteBuffer* mCurrent;
public:
    bool mAddingReleaseBuffer;
    bool mGettingReleaseBuffer;
};

class FiFoBuffering :
        virtual public BufferingPolicy
{
public:
    FiFoBuffering ();
    virtual ~FiFoBuffering ();
    virtual bool add ( ByteBuffer* buffer );
    virtual void get ( ByteBuffer*& buffer );
    virtual void get ( QList < ByteBuffer* >& buffer );
    virtual bool available ();
    virtual void clear ();
protected:
    QQueue < ByteBuffer* > mBuffers;
public:
    bool mGettingReleaseBuffer;
};

class LiFoBuffering :
        virtual public BufferingPolicy
{
public:
    LiFoBuffering ();
    virtual ~LiFoBuffering ();
    virtual bool add ( ByteBuffer* buffer );
    virtual void get ( ByteBuffer*& buffer );
    virtual void get ( QList < ByteBuffer* >& buffer );
    virtual bool available ();
    virtual void clear ();
protected:
    QStack < ByteBuffer* > mBuffers;
    bool mGettingReleaseBuffer;
};

#endif // NETWORKBUFFER_H
