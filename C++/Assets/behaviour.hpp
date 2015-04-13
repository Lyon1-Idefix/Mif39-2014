#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "AssetInterfaces/interfaces.hpp"
#include "AssetGenerics/generics.hpp"

namespace Assets {

class Behaviour;
class BehaviourLoader;
class BehaviourManager;

typedef QSharedPointer < Behaviour > BehaviourPtr;
typedef QSharedPointer < BehaviourLoader > BehaviourLoaderPtr;
typedef QSharedPointer < BehaviourManager > BehaviourManagerPtr;

typedef QList < QSharedPointer < Behaviour > > BehaviourList;

class BehaviourAttribute :
        virtual public IResource
{
public:
    BehaviourAttribute () {}
};

class Behaviour :
        virtual public SharedResource < Behaviour >
{
public:
    virtual ~Behaviour ();
protected:
    Behaviour () {}
    Behaviour (QString name);
    friend class ResourceDescriptor < Behaviour >;
};

class BehaviourLoader :
        virtual public SharedResourceLoader < Behaviour >
{
public:
    BehaviourLoader ();
    virtual ~BehaviourLoader ();
};

class BehaviourManager :
        virtual public SharedResourceManager < Behaviour >,
        virtual public Singleton < BehaviourManager >
{
public:
    BehaviourPtr getByUUID ( QUuid uuid );
protected:
    BehaviourManager();
    friend class Singleton < BehaviourManager >;
private:
};

}

#endif // BEHAVIOUR_H
