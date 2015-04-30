#ifndef WORLDCELL_H
#define WORLDCELL_H

#include "AssetInterfaces/interfaces.hpp"
#include "AssetGenerics/generics.hpp"

namespace Assets {

class WorldCell;
class WorldCellLoader;
class WorldCellManager;

typedef QSharedPointer < WorldCell > WorldCellPtr;
typedef QSharedPointer < WorldCellLoader > WorldCellLoaderPtr;
typedef QSharedPointer < WorldCellManager > WorldCellManagerPtr;

typedef QList < QSharedPointer < WorldCell > > WorldCellList;

class WorldCell :
        virtual public SharedResource < WorldCell >
{
public:
    virtual ~WorldCell ();
    virtual void Usage ();
protected:
    WorldCell ();
    WorldCell (QString name);
    virtual void _createAttributes ();
    virtual ByteBuffer _toBuffer ();
    virtual unsigned long long _fromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 );
    friend class ResourceDescriptor < WorldCell >;
};

class WorldCellManager :
        virtual public SharedResourceManager < WorldCell >,
        virtual public Singleton < WorldCellManager >
{
public:
    WorldCellPtr getByUUID ( QUuid uuid );
protected:
    WorldCellManager();
    friend class Singleton < WorldCellManager >;
private:
};

}

#endif // WORLDCELL_H
