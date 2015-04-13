#ifndef WRAPPER_H
#define WRAPPER_H

#include "Utility/includes.h"

template <class T> class Wrapper;

template <class T>
class Counted
{
        friend class Wrapper<T>;
private:
        Counted(T* pT) : Count(0), my_pT(pT)
                           { /*assert(pT != 0);*/ }
        template <class U> Counted(U* pT) : Count(0), my_pT(dynamic_cast<T*>(pT))
                           { /*assert(pT != 0);*/ }
        ~Counted()
        {
            assert(Count == 0);
            delete my_pT;
        }

        unsigned GetRef()  { return ++Count; }
        unsigned FreeRef() { assert(Count!=0); return --Count; }
        unsigned getCount () const { return Count; }

        unsigned Count;
        T* const my_pT;
};

template <class T>
class Wrapper
{
public:
        Wrapper()
            : m_pCounted(0)
        {
        }
        Wrapper(T* pT)
        {
                m_pCounted = new Counted<T>(pT);
                m_pCounted->GetRef();
        }
        ~Wrapper()
        {
                UnBind();
        }
        Wrapper(const Wrapper<T>& rVar)
        {
                m_pCounted = rVar.m_pCounted;
                if (!Null())
                        m_pCounted->GetRef();
        }
        template<class U> Wrapper(U* pT)
        {
                m_pCounted = new Counted<T>(dynamic_cast < T* > ( pT ) );
                m_pCounted->GetRef();
        }
        template < class U > Wrapper(const Wrapper<U>& rVar)
        {
                m_pCounted = (Counted<T>*) ( rVar.m_pCounted );
                if (!Null())
                        m_pCounted->GetRef();
        }

        Wrapper<T>& operator=(const Wrapper<T>& rVar)
        {
                if (!rVar.Null())
                    rVar.m_pCounted->GetRef();
                UnBind();
                m_pCounted = rVar.m_pCounted;
                return *this;
        }

        T* operator->()
        {
                if (Null())
                    throw std::exception();
                return m_pCounted->my_pT;
        }
        const T* operator->() const
        {
                if (Null())
                    throw std::exception();
                return m_pCounted->my_pT;
        }
        bool Null() const
        {
            return m_pCounted == 0;
        }
        void SetNull()
        {
            UnBind();
        }
        unsigned getCount () const
        {
            return m_pCounted->getCount();
        }
        void UnBind()
        {
                if (!Null() && m_pCounted->FreeRef() == 0)
                        delete m_pCounted;
                m_pCounted = 0;
        }
        Counted<T>* m_pCounted;
        bool equals ( const Wrapper < T >& rhs ) {
            return m_pCounted->my_pT == rhs.m_pCounted->my_pT;
        }
};

template<class T>
bool operator==(const Wrapper<T>& lhs,
                        const Wrapper<T>& rhs)
{
        return lhs.equals ( rhs );
        // or *(lhs.m_pCounted->my_pT) == *(rhs.....)
}

template<class T>
bool operator!=(const Wrapper<T>& lhs,
                        const Wrapper<T>& rhs)
{
        return !(lhs == rhs);
}

#endif // WRAPPER_H
