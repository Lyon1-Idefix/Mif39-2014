
#include "QImageLoader/qimageloader.hpp"
#include "Wavefront/wavefront.hpp"

#include "Networking/networking.hpp"
#include "TcpNetworking/simpletcpstartpoint.hpp"

typedef struct {
    QUuid uuid;
    SimpleTcpStartPoint* server;
    FiFoBuffering recv_buffering, send_buffering;
    pthread_t recv_thread, send_thread;
    bool closeMe;
} ClientData;

void* client_thread_receive ( void* data );
void* client_thread_send ( void* data );

SimpleTcpStartPoint* theServer;
QMap < QUuid, ClientData* > theClients;

void OnConnect ( QUuid client ) {
    std::cout << "Client " << client.toString().toStdString() << " has connected!!" << std::endl;
    ClientData* clt = new ClientData;
    clt->uuid = client;
    clt->server = theServer;
    clt->closeMe = false;
    clt->recv_buffering.mGettingReleaseBuffer = true;
    clt->send_buffering.mGettingReleaseBuffer = true;
    pthread_create ( &(clt->recv_thread), NULL, client_thread_receive, clt );
    pthread_create ( &(clt->send_thread), NULL, client_thread_send, clt );
    theClients [ client ] = clt;
}

void OnDisconnect ( QUuid client ) {
    std::cout << "Client " << client.toString().toStdString() << " has been disconnected!!" << std::endl;
    theClients [ client ]->closeMe = true;
    pthread_join(theClients [ client ]->recv_thread, NULL );
    pthread_join(theClients [ client ]->send_thread, NULL );
    theClients.remove( client );
}

void OnReceive ( QUuid client ) {
    ByteBuffer* message;
    theClients [ client ]->recv_buffering.get(message);
    if ( message->getLength() > 0 ) {
        std::cout << "Recv : " << message->getLength() << " bytes from " << theClients [ client ]->uuid.toString().toStdString() << std::endl;
        // gestion du message recu cote serveur
        unsigned long long index = 0;
        SharedResourcePtr mesh = ResourceHolder::FromBuffer(*message, index);
        std::cout << "Got mesh" << std::endl;
    }
    delete message;
}

void* client_thread_receive ( void* data )
{
    ClientData* client = (ClientData*) data;
    ByteBuffer* message = new ByteBuffer;
    std::cout << "Start client " << client->uuid.toString().toStdString() << " reception thread" << std::endl;
    while ( ! client->closeMe ) {
        if ( client->server->dataAvailable(client->uuid))
            client->server->receive(client->uuid,*message);
        if ( message->getLength() > 0 ) {
            client->recv_buffering.add(message);
            message = new ByteBuffer;
            OnReceive ( client->uuid );
        }
        pthread_yield_np();
    }
    return NULL;
}

void* client_thread_send ( void* data )
{
    ClientData* client = (ClientData*) data;
    std::cout << "Start client " << client->uuid.toString().toStdString() << " emission thread" << std::endl;
    while ( ! client->closeMe ) {
        if ( client->send_buffering.available() ) {
            ByteBuffer* message;
            client->send_buffering.get(message);
            client->server->send ( client->uuid, *message );
            delete message;
        }
        pthread_yield_np();
    }
    return NULL;
}

void* listen_thread ( void* data )
{
    QUuid fake;
    SimpleTcpStartPoint* server = (SimpleTcpStartPoint*) data;
    while ( true ) {
        QUuid client;
        while ( client == fake ) {
            client = server->listen();
        }
        OnConnect ( client );
        pthread_yield_np();
    }
    return NULL;
}

int main ( int argc, char** argv ) {
    QUuid fake;
    SimpleTcpStartPoint::Options options;
    options.connectionPort = 3000;
    options.maximumConnectedClients = 1;
    options.cbDisconnect = OnDisconnect;
    theServer = new SimpleTcpStartPoint ( options );
    theServer->start();
    pthread_t listen;
    pthread_create ( & listen, NULL, listen_thread, theServer );
    while ( true ) {
        pthread_yield_np();
    }
    theServer->stop ();
    return 0;
}
