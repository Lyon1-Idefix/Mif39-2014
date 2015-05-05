#include "resourceholder.hpp"

QMap < QString, SharedResourceLoaderPtr > ResourceHolder::mResourceLoaders;
QMap < QUuid, BaseResourceDescriptor* > ResourceHolder::mResourceDescriptors;

QMap < QUuid, SharedResourceList > ResourceHolder::mLibraries;
QMap < QUuid, SharedResourcePtr > ResourceHolder::mResources;

SharedResourcePtr ResourceHolder::CreateByName ( QString name, QString resname ) {
    QUuid type = UUIDManager::createUUID( "AssetImplementation::" + name );
    BaseResourceDescriptor* desc = mResourceDescriptors [ type ];
    SharedResourcePtr result = desc->Create(resname);
    result->mTypeID = desc->mResourceTypeID;
    if ( mResources.find(result->getUUID()) != mResources.end () )
        std::cerr << "ERROR: UUID " << result->getUUID().toString().toStdString() << " already registered for " << result->getName().toStdString() << std::endl;
    mResources [ result->getUUID () ] = result;
    mResourceDescriptors [ result->mTypeID ]->mManager->mLoadedResources [ result->getUUID() ] = result;
    return result;
}

QString ResourceHolder::GetTypeByUUID ( QUuid uuid ) {
    return mResourceDescriptors [ uuid ]->mResourceClassName;
}

SharedResourcePtr ResourceHolder::GetByUUID ( QUuid uuid ) {
    return mResources [ uuid ];
}

SharedResourceList ResourceHolder::GetAllByTypeName ( QString name ) {
    QUuid type = UUIDManager::createUUID( "AssetImplementation::" + name );
    BaseResourceDescriptor* desc = mResourceDescriptors [ type ];
    return mResourceDescriptors [ desc->mResourceTypeID ]->mManager->mLoadedResources.values();
}

QList < QUuid > ResourceHolder::AllKeys () { return mResources.keys(); }

SharedResourceList ResourceHolder::Load ( FileDescriptor filename ) {
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
    /*
    foreach ( SharedResourcePtr res, lib ) {
        //res->InitializeUUID("Assets:"+filename.fullFilename+":"+res->getName());
        mResources [ res->getUUID () ] = res;
        mResourceDescriptors [ res->mTypeID ]->mManager->mLoadedResources [ res->getUUID() ] = res;
    }
    */
    return lib;
}

SharedResourcePtr ResourceHolder::FromBuffer ( const ByteBuffer& buffer, unsigned long long& index ) {
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
    result->mTypeID = desc->mResourceTypeID;
    index = lindex;
    return result;
}

ByteBuffer ResourceHolder::ToBuffer ( const SharedResourcePtr& res ) {
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

void ResourceHolder::Export ( QString filename ) {
    ByteBuffer dictionary ( sizeof ( unsigned int ) );
    std::ofstream out1;
    out1.open ( filename.toStdString() );
    unsigned int i = 0;
    ::toBuffer (dictionary, 0, (uint) mResources.keys().size() );
    foreach ( QUuid uuid, mResources.keys() ) {
        ByteBuffer buffer = ResourceHolder::ToBuffer(mResources[uuid]);
        ByteBuffer buuid = ::toBuffer ( uuid );
        ByteBuffer blength = ::toBuffer ( buffer.getLength() );
        dictionary.append ( buuid ); dictionary.append ( blength );
        i ++;
    }
    out1.write((char*)dictionary.getData(),dictionary.getLength() );
    unsigned long long tmp = 0;
    i = 0;
    foreach ( QUuid uuid, mResources.keys() ) {
        ByteBuffer buffer = ResourceHolder::ToBuffer(mResources[uuid]);
        out1.write((char*)buffer.getData(),buffer.getLength() );
        tmp += buffer.getLength();
        i ++;
    }
    out1.close ();
    std::cout << "Count " << mResources.keys().size() << " bytes" << std::endl;
    std::cout << "Dict Size " << dictionary.getLength() << " bytes" << std::endl;
    std::cout << "Data Size " << tmp << " bytes" << std::endl;
    std::cout << "Total Size " << (dictionary.getLength()+tmp) << " bytes" << std::endl;
}

void ResourceHolder::RegisterResource ( SharedResourcePtr resource )
{
    if ( mResources.find(resource->getUUID()) != mResources.end () )
        std::cerr << "ERROR: UUID " << resource->getUUID().toString().toStdString() << " already registered for " << resource->getName().toStdString() << std::endl;
    mResources [ resource->getUUID () ] = resource;
    mResourceDescriptors [ resource->mTypeID ]->mManager->mLoadedResources [ resource->getUUID() ] = resource;
}

void ResourceHolder::Import ( QString filename ) {
    QList < QUuid > uuids;
    QMap < QUuid, unsigned long long > lengths;
    ByteBuffer count ( sizeof ( uint ) );
    ByteBuffer uuid ( 38 );
    ByteBuffer length ( sizeof ( unsigned long long ) );
    std::ifstream in1;
    unsigned long long tmpd = 0;
    in1.open ( filename.toStdString() );
    in1.read((char*)count.getData(), count.getLength());
    tmpd+= count.getLength();
    uint n; ::fromBuffer<uint> ( count, 0, n );
    for ( uint i = 0 ; i < n ; i ++ ) {
        QUuid v;
        unsigned long long l;
        in1.read((char*)uuid.getData(), uuid.getLength());
        tmpd+= uuid.getLength();
        in1.read((char*)length.getData(), length.getLength());
        tmpd+= length.getLength();
        ::fromBuffer ( uuid, 0, v );
        ::fromBuffer ( length, 0, l );
        uuids.append(v);
        lengths[v] = l;
    }
    unsigned long long tmp = 0;
    uint i = 0;
    for ( uint i = 0 ; i < n ; i ++ ) {
        QUuid v = uuids[i];
        unsigned long long l = lengths [v];
        unsigned long long index = 0;
        ByteBuffer b ( l );
        in1.read((char*)b.getData(), b.getLength());
        tmp += b.getLength();
        SharedResourcePtr result = ResourceHolder::FromBuffer(b,index);
        RegisterResource ( result );
    }
    std::cout << "Count " << n << " bytes" << std::endl;
    std::cout << "Dict Size " << tmpd << " bytes" << std::endl;
    std::cout << "Data Size " << tmp << " bytes" << std::endl;
    std::cout << "Total Size " << (tmpd+tmp) << " bytes" << std::endl;

    in1.close ();
}

void ResourceHolder::Usage () {
    std::cout << mResourceLoaders.size() << std::endl;
    std::cout << mResourceDescriptors.size() << std::endl;
    std::cout << mResources.size() << std::endl;
}
