#ifndef ISHAREDRESOURCEMANAGER_HPP
#define ISHAREDRESOURCEMANAGER_HPP

#include "AssetInterfaces/internals.hpp"

class ISharedResourceManager :
        virtual public HasUUID
{
public:
    /*
    static SharedResourcePtr GetByUUID ( QUuid uuid );
    static void RegisterManager ( SharedResourceManagerPtr mgr ) {
        mRegisteredManagers [ UUIDManager::createUUID(mgr->mMimePattern) ] = mgr;
    }
    static void RegisterLoader(QMimeType type, SharedResourceLoaderPtr loader);
    static SharedResourceList LoadLibrary ( FileDescriptor filename );
    static void Usage ();
    static QList < QUuid > AllKeys () { return mAllResources.keys(); }
    */
protected:
    ISharedResourceManager(QString pattern);
    SharedResourcePtr getByUUID ( QUuid uuid );
    void registerLoader (QMimeType type, SharedResourceLoaderPtr loader);
    SharedResourceList loadLibrary ( FileDescriptor filename );
    void usage () {
        std::cout << mMimePattern.toStdString()
                  << ":: #loaders: " << mRegisteredLoaders.size()
                  << ", #libs: " << mLoadedLibraries.size()
                  << ", #res: " << mLoadedResources.size() << std::endl;
    }
    //
    QString mMimePattern;
    SharedResourceLoaderDictionnary mRegisteredLoaders;
    SharedResourceLibraryDictionnary mLoadedLibraries;
    SharedResourceDictionnary mLoadedResources;
    //
    //static SharedResourceDictionnary mAllResources;
    //static SharedResourceManagerDictionnary mRegisteredManagers;
    friend class ResourceHolder;
};

#endif // ISHAREDRESOURCEMANAGER_HPP

