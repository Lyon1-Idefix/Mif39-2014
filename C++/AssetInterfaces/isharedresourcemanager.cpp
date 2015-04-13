#include "AssetInterfaces/isharedresourcemanager.hpp"

#include "AssetInterfaces/isharedresource.hpp"
#include "AssetInterfaces/isharedresourceloader.hpp"

ISharedResourceManager::ISharedResourceManager(QString pattern) :
    mMimePattern ( pattern )
{
    HasUUID::InitializeUUID(pattern);
}

SharedResourcePtr ISharedResourceManager::getByUUID ( QUuid uuid )
{
    SharedResourcePtr result;
    if ( mLoadedResources.find(uuid) != mLoadedResources.end () )
        result = mLoadedResources [ uuid ];
    return result;
}

void ISharedResourceManager::registerLoader (QMimeType type, SharedResourceLoaderPtr loader)
{
    mRegisteredLoaders [ UUIDManager::createUUID( type.name() ) ] = loader;
}

SharedResourceList ISharedResourceManager::loadLibrary ( FileDescriptor filename )
{
    QMimeDatabase db;
    QUuid uuidFile = UUIDManager::createUUID( filename.fullFilename );
    if ( mLoadedLibraries.find ( uuidFile ) != mLoadedLibraries.end () )
        return mLoadedLibraries [ uuidFile ];
    SharedResourceList lib;
    SharedResourceLoaderPtr loader;
    QMimeType mimeType = db.mimeTypeForFile(filename.fullFilename);
    QUuid uuidType = UUIDManager::createUUID( mimeType.name() );
    if ( mRegisteredLoaders.find ( uuidType ) != mRegisteredLoaders.end () )
        loader = mRegisteredLoaders [ uuidType ];
    if ( ! loader.isNull() )
        lib = loader->load(filename);
    mLoadedLibraries[uuidFile] = lib;
    foreach ( SharedResourcePtr res, lib ) {
        mLoadedResources [ res->getUUID() ] = res;
    }
    return lib;
}
//////////////////////////////////

/*
SharedResourcePtr ISharedResourceManager::GetByUUID ( QUuid uuid ) {
    SharedResourcePtr result;
    if ( mAllResources.find(uuid) != mAllResources.end () )
        result = mAllResources [ uuid ];
    return result;
}

void ISharedResourceManager::RegisterLoader(QMimeType type, SharedResourceLoaderPtr loader) {
    foreach ( SharedResourceManagerPtr mgr, mRegisteredManagers.values() ) {
        if ( type.name ().startsWith( mgr->mMimePattern ) ) {
            mgr->registerLoader(type,loader);
        }
    }
}

SharedResourceList ISharedResourceManager::LoadLibrary ( FileDescriptor filename ) {
    SharedResourceList result;
    QMimeDatabase db;
    QMimeType mimeType = db.mimeTypeForFile(filename.fullFilename);
    foreach ( SharedResourceManagerPtr mgr, mRegisteredManagers.values() ) {
        if ( mimeType.name ().startsWith( mgr->mMimePattern ) ) {
            SharedResourceList result = mgr->loadLibrary(filename);
            foreach ( SharedResourcePtr res, result )
                mAllResources [ res->getUUID () ] = res;
            return result;
        }
    }
    return result;
}

void ISharedResourceManager::Usage () {
    foreach ( SharedResourceManagerPtr mgr, mRegisteredManagers.values() ) {
        std::cout << mgr.data() << std::endl;
        mgr->usage ();
    }
}

SharedResourceDictionnary ISharedResourceManager::mAllResources;
SharedResourceManagerDictionnary ISharedResourceManager::mRegisteredManagers;
*/
