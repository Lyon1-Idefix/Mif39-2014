#ifndef ASSET_INTERFACES_INTERNALS_HPP
#define ASSET_INTERFACES_INTERNALS_HPP

#include "Utility/utility.hpp"

class AttributeContainer;
class IResource;

class ISharedResource;
class ISharedResourceLoader;
class ISharedResourceManager;

typedef QSharedPointer < ISharedResource > SharedResourcePtr;
typedef QSharedPointer < ISharedResourceLoader > SharedResourceLoaderPtr;
typedef QSharedPointer < ISharedResourceManager > SharedResourceManagerPtr;

typedef QList < SharedResourcePtr > SharedResourceList;
typedef QList < SharedResourceLoaderPtr > SharedResourceLoaderList;
typedef QList < SharedResourceManagerPtr > SharedResourceManagerList;

typedef QMap < QUuid, SharedResourcePtr > SharedResourceDictionnary;
typedef QMap < QUuid, SharedResourceList > SharedResourceLibraryDictionnary;
typedef QMap < QUuid, SharedResourceLoaderPtr > SharedResourceLoaderDictionnary;
typedef QMap < QUuid, SharedResourceManagerPtr > SharedResourceManagerDictionnary;

#endif // ASSET_INTERFACES_INTERNALS_HPP

