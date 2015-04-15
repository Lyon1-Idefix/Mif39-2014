#ifndef RESOURCEHOLDER_H
#define RESOURCEHOLDER_H

#include "AssetInterfaces/internals.hpp"

#include "AssetInterfaces/isharedresource.hpp"
#include "AssetInterfaces/isharedresourceloader.hpp"
#include "AssetInterfaces/isharedresourcemanager.hpp"

class BaseResourceDescriptor
{
public:
    virtual ~BaseResourceDescriptor () {}
    QString mResourceClassName;
    QUuid mResourceTypeID;
    SharedResourceManagerPtr mManager;
    virtual SharedResourcePtr Create () = 0;
    virtual SharedResourcePtr Create (QString name) = 0;
protected:
    BaseResourceDescriptor () {}
};

template < class ResourceType >
class ResourceDescriptor :
        virtual public BaseResourceDescriptor
{
public:
    virtual ~ResourceDescriptor () {}
    QUuid mInterfaceID;
    static char* mImplementationName;
    virtual SharedResourcePtr Create () {
        SharedResourcePtr ptr ( new ResourceType );
        return ptr;
    }
    virtual SharedResourcePtr Create (QString name) {
        uint index=0;
        QString tmp = "Resource[" + mResourceClassName + ":" + name + "]";
        if ( mNameCount.find ( tmp ) != mNameCount.end () ) {
            index = mNameCount[tmp] + 1;
        }
        mNameCount[tmp] = index;
        QString sindex = QString::number(index);
        QString reference = "Resource[" + mResourceClassName + ":" + name + ":" + sindex + "]";
        QUuid uuid = UUIDManager::createUUID(reference);
        SharedResourcePtr ptr ( new ResourceType (reference) );
        ptr->setUUID ( uuid );
        return ptr;
    }
protected:
    ResourceDescriptor () {}
    QMap < QString, uint > mNameCount;
    friend class ResourceHolder;
};
template < class ResourceType > char* ResourceDescriptor<ResourceType>::mImplementationName;

template < class ResourceType >
class ResourceInterfaceDescriptor :
        virtual public BaseResourceDescriptor
{
public:
    virtual ~ResourceInterfaceDescriptor () {}
    static char* mInterfaceName;
    virtual SharedResourcePtr Create () {
        SharedResourcePtr ptr;
        return ptr;
    }
    virtual SharedResourcePtr Create (QString name) {
        SharedResourcePtr ptr;
        return ptr;
    }
protected:
    ResourceInterfaceDescriptor () {}
    friend class ResourceHolder;
};
template < class ResourceType > char* ResourceInterfaceDescriptor<ResourceType>::mInterfaceName;

class ResourceHolder
{
public:
    template < class ResourceType, class ManagerType > static void RegisterInterface ( QString name ) {
        SharedResourceManagerPtr mgr = ManagerType::Instance ();
        ResourceInterfaceDescriptor < ResourceType >* tmp = new ResourceInterfaceDescriptor < ResourceType >;
        tmp->mResourceClassName = name;
        tmp->mResourceTypeID = UUIDManager::createUUID( "AssetInterface::" + name );
        tmp->mManager = mgr;
        mResourceDescriptors [ tmp->mResourceTypeID ] = tmp;
        ResourceInterfaceDescriptor < ResourceType >::mInterfaceName = strdup ( name.toStdString().c_str() );
    }
    template < class ResourceType, class InterfaceType > static void RegisterImplementation ( QString name ) {
        QString interfaceName = ResourceInterfaceDescriptor < InterfaceType >::mInterfaceName;
        QUuid type = UUIDManager::createUUID( "AssetInterface::" + interfaceName );
        BaseResourceDescriptor* desc = mResourceDescriptors [ type ];
        ResourceDescriptor < ResourceType >* tmp = new ResourceDescriptor < ResourceType >;
        tmp->mInterfaceID = desc->mResourceTypeID;
        tmp->mResourceClassName = name;
        tmp->mResourceTypeID = UUIDManager::createUUID( "AssetImplementation::" + name );
        tmp->mManager = desc->mManager;
        mResourceDescriptors [ tmp->mResourceTypeID ] = tmp;
        ResourceDescriptor < ResourceType >::mImplementationName = strdup ( name.toStdString().c_str() );
    }
    template < class ResourceType, class LoaderType > static void RegisterLoader ( QMimeType fileType ) {
        SharedResourceLoaderPtr ldr = LoaderType::Instance ();
        mResourceLoaders [ fileType.name () ] = ldr;
    }

    ///
    /// \brief CreateByUUID
    /// \param type, UUID of resource type
    /// \return shared pointer on the resource
    ///
    static SharedResourcePtr CreateByUUID ( QUuid type ) {
        BaseResourceDescriptor* desc = mResourceDescriptors [ type ];
        SharedResourcePtr result = desc->Create();
        result->mTypeID = desc->mResourceTypeID;
        return result;
    }
    ///
    /// \brief CreateByName
    /// \param name, name of resource type
    /// \return shared pointer on the resource
    ///
    static SharedResourcePtr CreateByName ( QString name ) {
        QUuid type = UUIDManager::createUUID( "AssetImplementation::" + name );
        BaseResourceDescriptor* desc = mResourceDescriptors [ type ];
        SharedResourcePtr result = desc->Create();
        result->mTypeID = desc->mResourceTypeID;
        return result;
    }
    ///
    /// \brief CreateByName
    /// \param name, name of resource type
    /// \param resname, name of resource
    /// \return shared pointer on the resource
    ///
    static SharedResourcePtr CreateByName ( QString name, QString resname ) {
        QUuid type = UUIDManager::createUUID( "AssetImplementation::" + name );
        BaseResourceDescriptor* desc = mResourceDescriptors [ type ];
        SharedResourcePtr result = desc->Create(resname);
        result->mTypeID = desc->mResourceTypeID;
        return result;
    }
    ///
    /// \brief GetByUUID
    /// \param uuid, UUID of the requested resource
    /// \return shared pointer on the resource
    ///
    static SharedResourcePtr GetByUUID ( QUuid uuid ) {
        return mResources [ uuid ];
    }
    ///
    /// \brief AllKeys
    /// \return
    ///
    static QList < QUuid > AllKeys () { return mResources.keys(); }
    ///
    /// \brief Load
    /// \param filename
    /// \return
    ///
    static SharedResourceList Load ( FileDescriptor filename ) {
        QMimeDatabase db;
        QUuid uuidFile = UUIDManager::createUUID( filename.fullFilename );
        if ( mLibraries.find ( uuidFile ) != mLibraries.end () )
            return mLibraries [ uuidFile ];
        SharedResourceList lib;
        SharedResourceLoaderPtr loader;
        QMimeType mimeType = db.mimeTypeForFile(filename.fullFilename);
        if ( mResourceLoaders.find ( mimeType.name () ) != mResourceLoaders.end () )
            loader = mResourceLoaders [ mimeType.name () ];
        if ( ! loader.isNull() )
            lib = loader->load(filename);
        mLibraries[uuidFile] = lib;
        foreach ( SharedResourcePtr res, lib ) {
            res->InitializeUUID("Assets:"+filename.fullFilename+":"+res->getName());
            mResources [ res->getUUID () ] = res;
            mResourceDescriptors [ res->mTypeID ]->mManager->mLoadedResources [ res->getUUID() ] = res;
        }
        return lib;
    }

    static SharedResourcePtr FromBuffer ( const ByteBuffer& buffer, unsigned long long& index ) {
        QUuid resTypeUUID;
        QString resTypeName;
        unsigned int dataSize;
        unsigned long long lindex = index;
        lindex = ::fromBuffer ( buffer, lindex, resTypeUUID );
        lindex = ::fromBuffer ( buffer, lindex, resTypeName );
        lindex = ::fromBuffer ( buffer, lindex, dataSize );
        BaseResourceDescriptor* desc = mResourceDescriptors [ resTypeUUID ];
        SharedResourcePtr result ( desc->Create() );
        lindex = result->convertFromBuffer ( buffer, lindex );
        index = lindex;
        return result;
    }
    static ByteBuffer ToBuffer ( const SharedResourcePtr& res ) {
        BaseResourceDescriptor* desc = mResourceDescriptors [ res->mTypeID ];
        ByteBuffer uuid = toBuffer ( desc->mResourceTypeID );
        ByteBuffer name = toBuffer ( desc->mResourceClassName );
        ByteBuffer data = res->convertToBuffer();
        unsigned int dataSize = data.getLength();
        unsigned long long index = 0;
        ByteBuffer buffer ( uuid.getLength() + name.getLength() + sizeof ( unsigned int ) + dataSize );
        index = ::toBuffer ( buffer, index, uuid.getData(), uuid.getLength() );
        index = ::toBuffer ( buffer, index, name.getData(), name.getLength() );
        index = ::toBuffer ( buffer, index, dataSize );
        index = ::toBuffer ( buffer, index, data.getData(), data.getLength() );
        return buffer;
    }

    ///
    /// \brief Usage
    ///
    static void Usage () {
        std::cout << mResourceLoaders.size() << std::endl;
        std::cout << mResourceDescriptors.size() << std::endl;
        std::cout << mResources.size() << std::endl;
    }
protected:
    static QMap < QString, SharedResourceLoaderPtr > mResourceLoaders;
    static QMap < QUuid, BaseResourceDescriptor* > mResourceDescriptors;
    //
    static QMap < QUuid, SharedResourceList > mLibraries;
    static QMap < QUuid, SharedResourcePtr > mResources;
};

#endif // RESOURCEHOLDER_H
