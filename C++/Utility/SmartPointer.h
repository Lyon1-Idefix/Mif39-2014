#ifndef SMARTPOINTER_H
#define SMARTPOINTER_H

#if 1
#define LOGSP printf ( "[%20s:%04d] : 0x%08x:0x%08x(%02d)\n", __FUNCTION__, __LINE__, pData, reference, reference ? reference->getCount () : -1 );
#else
#define LOGSP if ( pData == NULL ) \
printf ( "[%20s:%04d] : 0x%08x:0x%08x(%02d)\n", __FUNCTION__, __LINE__, pData, reference, reference ? reference->getCount () : 0 ); \
else \
printf ( "[%20s:%04d] : 0x%08x:0x%08x(%02d) : %20s[%04d] : %s\n", __FUNCTION__, __LINE__, pData, reference, reference ? reference->getCount () : 0, pData->m_kind.toAscii ().data () )
#endif

#define LOG(m) printf ( "[%20s:%04d] : %s\n", __FUNCTION__, __LINE__, m )

#define LOGC(m) printf ( "[%20s:%04d] (0x%08x) : %s\n", __FUNCTION__, __LINE__, this, m )

class ReferenceCounting
{
    private:
    int count; // Reference count

    public:
    ReferenceCounting () : count ( 0 ) {
    }
    ~ReferenceCounting () {
    }

    void AddRef()
    {
        // Increment the reference count
        count++;
    }

    int Release()
    {
        // Decrement the reference count and
        // return the reference count.
        return --count;
    }
    int getCount () const { return count; }
};

template < typename T > class SmartPointer
{
public:
    T*    pData;       // pointer
    ReferenceCounting* reference; // Reference count

public:
    SmartPointer() : pData(0), reference(0)
    {
    }
/*
    explicit SmartPointer(T* pValue) : pData(pValue), reference(0)
    {
        reference = new ReferenceCounting;
        reference->AddRef();
    }*/
    template < class U > explicit SmartPointer(U* pValue) : pData(pValue), reference(0)
    {
        reference = new ReferenceCounting;
        reference->AddRef();
    }

    SmartPointer(const SmartPointer<T>& sp) : pData(sp.pData), reference(sp.reference)
    {
        if ( reference ) reference->AddRef();
    }
    template < class U > SmartPointer(const SmartPointer<U>& sp) : pData ( 0 ), reference(sp.reference)
    {
        pData = dynamic_cast < T* > ( sp.pData );
        if ( reference ) reference->AddRef();
    }

    ~SmartPointer()
    {
        if(reference && reference->Release() == 0)
        {
            delete pData;
            delete reference;
        }
    }

    T& operator* ()
    {
        return *pData;
    }

    T* operator-> ()
    {
        return pData;
    }

    SmartPointer<T>& operator = (const SmartPointer<T>& sp)
    {
        // Assignment operator
        if (this != &sp) // Avoid self assignment
        {
            if(reference && reference->Release() == 0)
            {
                LOGSP;
                delete pData;
                delete reference;
            }
            pData = sp.pData;
            if ( pData ) {
                reference = sp.reference;
                if ( reference ) reference->AddRef();
            }
            LOGSP;
        }
        return *this;
    }
    int getCount () const { return reference->getCount(); }
};

#include <boost/shared_ptr.hpp>

//#define SMARTPTR(T) T*
//#define SMARTPTR(T) boost::shared_ptr<T>
//#define SMARTPTR(T) SmartPointer < T >

#endif // SMARTPOINTER_H
