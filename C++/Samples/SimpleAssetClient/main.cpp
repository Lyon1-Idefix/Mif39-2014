
#include "QImageLoader/qimageloader.hpp"
#include "Wavefront/wavefront.hpp"

#include "TcpNetworking/simpletcpendpoint.hpp"

extern void __attach(void);
extern void __attachInterfaces(void);
extern void __attachGenerics(void);
extern void __attachAssets(void);
extern void __attachQImage(void);
extern void __attachWavefront(void);

QMap < QUuid, SharedResourcePtr > recvResources;

typedef void (ReceptionCallback) ( SharedResourcePtr resource );
QMap < QUuid, ReceptionCallback* > recvCB;

QQueue < QUuid > resourceToFetch;

QUuid receiveUUID (SimpleTcpEndPoint client)
{
    unsigned long long index = 0;
    QUuid result;
    ByteBuffer message;
    client.receive ( message );
    ::fromBuffer ( message, index, result );
    return result;
}

void sendUUID ( SimpleTcpEndPoint client, QUuid uuid )
{
    ByteBuffer buffer = ::toBuffer (uuid);
    client.send(buffer);
}

unsigned long long received = 0;

SharedResourcePtr receiveResource ( SimpleTcpEndPoint client )
{
    unsigned long long index = 0;
    ByteBuffer message;
    client.receive ( message );
    received += message.getLength();
    SharedResourcePtr result = ResourceHolder::FromBuffer(message,index);
    recvResources[result->getUUID()] = result;
    ResourceHolder::RegisterResource(result);
    if ( recvCB.find ( result->mTypeID) != recvCB.end () ) {
        (*recvCB[result->mTypeID]) ( result );
    }
    return result;
}

void fetchResource ( QUuid uuid )
{
    if ( recvResources.find(uuid) == recvResources.end () )
        resourceToFetch.enqueue(uuid);
}

void OnReceiveWorld ( SharedResourcePtr resource )
{
    std::cout << "World::" << resource->getName().toStdString() << std::endl;
    Assets::WorldPtr world = resource.dynamicCast<Assets::World> ();
    for ( int i = 0 ; i < world->getSize < QUuid > ( "Cells" ) ; i ++ ) {
        fetchResource ( world->get < QUuid > ( "Cells", i ) );
    }
}

void OnReceiveWorldCell ( SharedResourcePtr resource )
{
    std::cout << "WorldCell::" << resource->getName().toStdString() << std::endl;
    Assets::WorldCellPtr cell = resource.dynamicCast<Assets::WorldCell> ();
    for ( int i = 0 ; i < cell->getSize < QUuid > ( "GameObjects" ) ; i ++ ) {
        fetchResource ( cell->get < QUuid > ( "GameObjects", i ) );
    }
}

void OnReceiveGameObject ( SharedResourcePtr resource )
{
    std::cout << "GameObject::" << resource->getName().toStdString() << std::endl;
    Assets::GameObjectPtr go = resource.dynamicCast<Assets::GameObject> ();
    fetchResource ( go->get < QUuid > ( "Entity" ) );
}

void OnReceiveGameEntity ( SharedResourcePtr resource )
{
    std::cout << "GameEntity::" << resource->getName().toStdString() << std::endl;
    Assets::GameEntityPtr ge = resource.dynamicCast<Assets::GameEntity> ();
    for ( int i = 0 ; i < ge->getSize < Assets::GameEntityElement* > ( "Elements" ) ; i ++ ) {
        Assets::GameEntityElement* elt = ge->get < Assets::GameEntityElement* > ( "Elements", i );
        for ( int j = 0 ; j < elt->getSize < QUuid > ( "Resources" ) ; j ++ ) {
            fetchResource ( elt->get < QUuid > ( "Resources", j ) );
        }
    }
}

void OnReceiveLight ( SharedResourcePtr resource )
{
    std::cout << "Light::" << resource->getName().toStdString() << std::endl;
    Assets::LightPtr light = resource.dynamicCast<Assets::Light> ();
}

void OnReceiveImage ( SharedResourcePtr resource )
{
    std::cout << "Image::" << resource->getName().toStdString() << std::endl;
    Assets::ImagePtr image = resource.dynamicCast<Assets::Image> ();
}

void OnReceiveWavefrontMaterial ( SharedResourcePtr resource )
{
    QUuid fake;
    std::cout << "WavefrontMaterial::" << resource->getName().toStdString() << std::endl;
    Assets::MaterialPtr material = resource.dynamicCast<WavefrontMaterial> ();
    if ( fake != material->get < Texture > ( "AmbientMap" ).imageID )
        fetchResource ( material->get < Texture > ( "AmbientMap" ).imageID );
    if ( fake != material->get < Texture > ( "DiffuseMap" ).imageID )
        fetchResource ( material->get < Texture > ( "DiffuseMap" ).imageID );
    if ( fake != material->get < Texture > ( "SpecularMap" ).imageID )
        fetchResource ( material->get < Texture > ( "SpecularMap" ).imageID );
    if ( fake != material->get < Texture > ( "SpecularExponentMap" ).imageID )
        fetchResource ( material->get < Texture > ( "SpecularExponentMap" ).imageID );
    if ( fake != material->get < Texture > ( "DissolveMap" ).imageID )
        fetchResource ( material->get < Texture > ( "DissolveMap" ).imageID );
    if ( fake != material->get < Texture > ( "DecalMap" ).imageID )
        fetchResource ( material->get < Texture > ( "DecalMap" ).imageID );
    if ( fake != material->get < Texture > ( "DisplacementMap" ).imageID )
        fetchResource ( material->get < Texture > ( "DisplacementMap" ).imageID );
    if ( fake != material->get < Texture > ( "BumpMap" ).imageID )
        fetchResource ( material->get < Texture > ( "BumpMap" ).imageID );
    if ( fake != material->get < Texture > ( "ReflectionMap" ).imageID )
        fetchResource ( material->get < Texture > ( "ReflectionMap" ).imageID );
}

void OnReceiveMesh ( SharedResourcePtr resource )
{
    std::cout << "Mesh::" << resource->getName().toStdString() << std::endl;
    Assets::MeshPtr mesh = resource.dynamicCast<Assets::Mesh> ();
    for ( unsigned int i = 0 ; i < mesh->getSize < Assets::MaterialGroup* > ( "MaterialGroups" ) ; i ++ ) {
        Assets::MaterialGroup* grp = mesh->get < Assets::MaterialGroup* > ( "MaterialGroups", i );
        fetchResource ( grp->get < QUuid > ( "Material" ) );
    }
}

/*
Send-Import:
    Count 1658 bytes
    Dict Size 76272 bytes
    Data Size 91315785 bytes
    Total Size 91392057 bytes
Recv-Export:
    Count 1616 bytes
    Dict Size 74340 bytes
    Data Size 91309561 bytes
    Total Size 91383901 bytes
*/
int main ( int argc, char** argv ) {
    /*
    __attach();
    __attachInterfaces();
    __attachGenerics();
    __attachAssets();*/
    __attachQImage();
    __attachWavefront();

    recvCB [ UUIDManager::createUUID("AssetImplementation::World") ] = OnReceiveWorld;
    recvCB [ UUIDManager::createUUID("AssetImplementation::WorldCell") ] = OnReceiveWorldCell;
    recvCB [ UUIDManager::createUUID("AssetImplementation::GameObject") ] = OnReceiveGameObject;
    recvCB [ UUIDManager::createUUID("AssetImplementation::GameEntity") ] = OnReceiveGameEntity;
    recvCB [ UUIDManager::createUUID("AssetImplementation::Light") ] = OnReceiveLight;
    recvCB [ UUIDManager::createUUID("AssetImplementation::Image") ] = OnReceiveImage;
    recvCB [ UUIDManager::createUUID("AssetImplementation::WavefrontMaterial") ] = OnReceiveWavefrontMaterial;
    recvCB [ UUIDManager::createUUID("AssetImplementation::Mesh") ] = OnReceiveMesh;

    SimpleTcpEndPoint::Options options;
    options.serverIP = "127.0.0.1";
    options.connectionPort = 3000;
    SimpleTcpEndPoint client ( options );
    if ( client.open() == false ) exit ( -1 );
    // reception de l'UUID du monde
    QUuid worldID = receiveUUID(client);
    fetchResource(worldID);
    while ( true ) {
        if ( resourceToFetch.size() > 0 ) {
            QUuid uuid = resourceToFetch.dequeue();
            if ( recvResources.find(uuid)==recvResources.end () ) {
                sendUUID(client, uuid);
                receiveResource(client);
            }
        }
        else break;
    }
    std::cout << "Received " << received << " bytes" << std::endl;
    /*
    sendUUID(client,worldID);
    Assets::WorldPtr world = receiveResource(client).dynamicCast<Assets::World>();
    std::cout << world->mTypeID.toString().toStdString() << std::endl;
    std::cout << UUIDManager::createUUID("AssetImplementation::World").toString().toStdString() << std::endl;
    for ( int i = 0 ; i < world->getSize < QUuid > ( "Cells" ) ; i ++ ) {
        sendUUID(client,world->get < QUuid > ( "Cells", i ));
        Assets::WorldCellPtr cell = receiveResource(client).dynamicCast<Assets::WorldCell>();
    }
    */
    client.close ();
    ResourceHolder::Export( "world.binary" );
    return 0;
}
