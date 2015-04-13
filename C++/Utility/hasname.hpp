#ifndef HASNAME_H
#define HASNAME_H

#include "Utility/includes.hpp"

class HasName
{
public:
    inline QString getName () { return mName; }
    inline const QString getName () const { return mName; }
protected:
    HasName ();
    void Initialize ( QString name );
    QString mName;
};

#endif // HASNAME_H
