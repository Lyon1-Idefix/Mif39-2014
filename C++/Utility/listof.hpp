#ifndef LISTOF_HPP
#define LISTOF_HPP

#include "Utility/includes.hpp"

template < class ElementType > class ListOf
{
public:
    ListOf () {}
    void Add ( ElementType element ) {
        mData.append ( element );
    }

    const ElementType& operator [] ( int index ) const {
        return mData.at ( index );
    }
    ElementType& operator [] ( int index ) {
        return mData.at ( index );
    }
    unsigned int Size () const {
        return mData.size ();
    }
protected:
    QList < ElementType > mData;
};

#endif // LISTOF_HPP

