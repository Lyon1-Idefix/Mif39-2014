#ifndef RESOURCEHOLDER_H
#define RESOURCEHOLDER_H

#include "AssetInterfaces/internals.hpp"

#include "AssetInterfaces/isharedresource.hpp"
#include "AssetInterfaces/isharedresourceloader.hpp"
#include "AssetInterfaces/isharedresourcemanager.hpp"

#include <iostream>
#include <fstream>

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
        std::cout << "Interface: [" << name.toStdString() << "] --> [" << tmp->mResourceTypeID.toString().toStdString() << "]" << std::endl;
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
        std::cout << "Implementation: [" << name.toStdString() << "] --> [" << tmp->mResourceTypeID.toString().toStdString() << "]" << std::endl;
    }
    template < class ResourceType, class LoaderType > static void RegisterLoader ( QMimeType fileType ) {
        SharedResourceLoaderPtr ldr = LoaderType::Instance ();
        mResourceLoaders [ fileType.name () ] = ldr;
    }

    ///
    /// \brief CreateByName
    /// \param name, name of resource type
    /// \param resname, name of resource
    /// \return shared pointer on the resource
    ///
    static SharedResourcePtr CreateByName ( QString name, QString resname );
    static QString GetTypeByUUID ( QUuid uuid );
    ///
    /// \brief GetByUUID
    /// \param uuid, UUID of the requested resource
    /// \return shared pointer on the resource
    ///
    static SharedResourcePtr GetByUUID ( QUuid uuid );
    static SharedResourceList GetAllByTypeName ( QString name );
    ///
    /// \brief AllKeys
    /// \return
    ///
    static QList < QUuid > AllKeys ();
    ///
    /// \brief Load
    /// \param filename
    /// \return
    ///
    static SharedResourceList Load ( FileDescriptor filename );
    static SharedResourcePtr FromBuffer ( const ByteBuffer& buffer, unsigned long long& index );
    static ByteBuffer ToBuffer ( const SharedResourcePtr& res );
    static void Export ( QString filename );
    static void RegisterResource ( SharedResourcePtr resource );
    static void Import ( QString filename );

    ///
    /// \brief Usage
    ///
    static void Usage ();
protected:
    static QMap < QString, SharedResourceLoaderPtr > mResourceLoaders;
    static QMap < QUuid, BaseResourceDescriptor* > mResourceDescriptors;
    //
    static QMap < QUuid, SharedResourceList > mLibraries;
    static QMap < QUuid, SharedResourcePtr > mResources;
};

#endif // RESOURCEHOLDER_H
