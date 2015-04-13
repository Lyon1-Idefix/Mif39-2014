#include "resourceholder.hpp"

QMap < QString, SharedResourceLoaderPtr > ResourceHolder::mResourceLoaders;
QMap < QUuid, BaseResourceDescriptor* > ResourceHolder::mResourceDescriptors;

QMap < QUuid, SharedResourceList > ResourceHolder::mLibraries;
QMap < QUuid, SharedResourcePtr > ResourceHolder::mResources;
