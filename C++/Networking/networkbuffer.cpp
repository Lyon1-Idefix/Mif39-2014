#include "networkbuffer.hpp"

////////////////////////////////////////////////////////////

BufferingPolicy::BufferingPolicy()
{
}

BufferingPolicy::~BufferingPolicy()
{
}

////////////////////////////////////////////////////////////

SingleBuffering::SingleBuffering () :
    mCurrent ( NULL ),
    mAddingReleaseBuffer ( false ),
    mGettingReleaseBuffer ( false )
{
    pthread_mutex_init(&mBufferLock, NULL);
}

SingleBuffering::~SingleBuffering ()
{
}

bool SingleBuffering::add ( ByteBuffer* buffer )
{
    pthread_mutex_lock (&mBufferLock);
    if ( mAddingReleaseBuffer )
        mCurrent = NULL;
    if ( mCurrent == NULL )
        mCurrent = buffer;
    pthread_mutex_unlock (&mBufferLock);
    return mCurrent == buffer;
}

void SingleBuffering::get ( ByteBuffer*& buffer )
{
    pthread_mutex_lock (&mBufferLock);
    buffer = mCurrent;
    if ( mGettingReleaseBuffer )
        mCurrent = NULL;
    pthread_mutex_unlock (&mBufferLock);
}

void SingleBuffering::get ( QList < ByteBuffer* >& buffer )
{
    buffer.clear();
    pthread_mutex_lock (&mBufferLock);
    buffer.append( mCurrent );
    if ( mGettingReleaseBuffer )
        mCurrent = NULL;
    pthread_mutex_unlock (&mBufferLock);
}

bool SingleBuffering::available ()
{
    bool result;
    pthread_mutex_lock (&mBufferLock);
    result = mCurrent == NULL;
    pthread_mutex_unlock (&mBufferLock);
    return result;
}

void SingleBuffering::clear ()
{
    pthread_mutex_lock (&mBufferLock);
    mCurrent = NULL;
    pthread_mutex_unlock (&mBufferLock);
}

////////////////////////////////////////////////////////////

FiFoBuffering::FiFoBuffering () :
    mGettingReleaseBuffer ( false )
{
    pthread_mutex_init(&mBufferLock, NULL);
}

FiFoBuffering::~FiFoBuffering ()
{
}

bool FiFoBuffering::add ( ByteBuffer* buffer )
{
    pthread_mutex_lock (&mBufferLock);
    mBuffers.enqueue(buffer);
    pthread_mutex_unlock (&mBufferLock);
    return mBuffers.size() > 0;
}

void FiFoBuffering::get ( ByteBuffer*& buffer )
{
    pthread_mutex_lock (&mBufferLock);
    buffer = NULL;
    if ( mBuffers.size() > 0 )
        buffer = mBuffers.head();
    if ( mGettingReleaseBuffer )
        mBuffers.dequeue();
    pthread_mutex_unlock (&mBufferLock);
}

void FiFoBuffering::get ( QList < ByteBuffer* >& buffer )
{
    buffer.clear();
    pthread_mutex_lock (&mBufferLock);
    if ( mBuffers.size() > 0 ) {
        foreach (ByteBuffer* buf, mBuffers) {
            buffer.append(buf);
        }
    }
    if ( mGettingReleaseBuffer )
        mBuffers.clear();
    pthread_mutex_unlock (&mBufferLock);
}

bool FiFoBuffering::available ()
{
    bool result;
    pthread_mutex_lock (&mBufferLock);
    result = mBuffers.size() > 0;
    pthread_mutex_unlock (&mBufferLock);
    return result;
}

void FiFoBuffering::clear ()
{
    pthread_mutex_lock (&mBufferLock);
    mBuffers.clear();
    pthread_mutex_unlock (&mBufferLock);
}

////////////////////////////////////////////////////////////

LiFoBuffering::LiFoBuffering () :
    mGettingReleaseBuffer ( false )
{
    pthread_mutex_init(&mBufferLock, NULL);
}

LiFoBuffering::~LiFoBuffering ()
{
}

bool LiFoBuffering::add ( ByteBuffer* buffer )
{
    pthread_mutex_lock (&mBufferLock);
    mBuffers.push(buffer);
    pthread_mutex_unlock (&mBufferLock);
    return mBuffers.size() > 0;
}

void LiFoBuffering::get ( ByteBuffer*& buffer )
{
    pthread_mutex_lock (&mBufferLock);
    buffer = NULL;
    if ( mBuffers.size() > 0 )
        buffer = mBuffers.top();
    if ( mGettingReleaseBuffer )
        mBuffers.pop();
    pthread_mutex_unlock (&mBufferLock);
}

void LiFoBuffering::get ( QList < ByteBuffer* >& buffer )
{
    buffer.clear();
    pthread_mutex_lock (&mBufferLock);
    if ( mBuffers.size() > 0 ) {
        foreach (ByteBuffer* buf, mBuffers) {
            buffer.append(buf);
        }
    }
    if ( mGettingReleaseBuffer )
        mBuffers.clear();
    pthread_mutex_unlock (&mBufferLock);
}

bool LiFoBuffering::available ()
{
    bool result;
    pthread_mutex_lock (&mBufferLock);
    result = mBuffers.size() > 0;
    pthread_mutex_unlock (&mBufferLock);
    return result;
}

void LiFoBuffering::clear ()
{
    pthread_mutex_lock (&mBufferLock);
    mBuffers.clear();
    pthread_mutex_unlock (&mBufferLock);
}
