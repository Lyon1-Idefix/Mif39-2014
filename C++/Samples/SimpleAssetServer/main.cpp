#include "QImageLoader/qimageloader.hpp"
#include "Wavefront/wavefront.hpp"

#include "TcpNetworking/simpletcpstartpoint.hpp"

extern void __attach(void);
extern void __attachInterfaces(void);
extern void __attachGenerics(void);
extern void __attachAssets(void);
extern void __attachQImage(void);
extern void __attachWavefront(void);

int main ( int argc, char** argv ) {
    /*
    __attach();
    __attachInterfaces();
    __attachGenerics();
    __attachAssets();*/
    __attachQImage();
    __attachWavefront();

    ResourceHolder::Import( argv[1] );

    QUuid fake;
    SimpleTcpStartPoint::Options options;
    options.connectionPort = 3000;
    options.maximumConnectedClients = 1;
    options.cbDisconnect = NULL;
    SimpleTcpStartPoint server ( options );
    server.start();
    while ( true ) {
        // attente d'un client
        std::cout << "Wait for client" << std::endl;
        QUuid client;
        while ( client == fake ) {
            client = server.listen();
        }
        std::cout << "Client " << client.toString().toStdString() << " connected" << std::endl;
        // envoi l'UUID du 1er monde lors de la connexion d'un client
        QUuid world = ResourceHolder::GetAllByTypeName("World" ) [ 0 ]->getUUID();
        ByteBuffer bworld = ::toBuffer (world);
        if ( server.send ( client, bworld ) ) {
            unsigned long long sended = 0;
            while ( true ) {
                // attente d'une requete du client
                ByteBuffer request;
                QUuid uuid;
                unsigned long long index = 0;
                if ( ! server.receive(client,request) )
                    break;
                index = ::fromBuffer(request,index, uuid);
                std::cout << "Request for " << uuid.toString().toStdString() << "..."; std::cout.flush();
                // recuperation de la ressource
                SharedResourcePtr resource = ResourceHolder::GetByUUID(uuid);
                // conversion binaire
                ByteBuffer reply = ResourceHolder::ToBuffer(resource);
                // envoi de la reponse
                if ( ! server.send(client,reply) )
                    break;
                sended += reply.getLength();
                std::cout << " replied" << std::endl;
            }
            std::cout << "Sent " << sended << " bytes" << std::endl;
        }
        // deconnexion du client
    }
    server.stop ();
    return 0;
}
