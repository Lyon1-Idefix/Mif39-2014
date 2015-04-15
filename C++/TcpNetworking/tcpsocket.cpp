#include "tcpsocket.hpp"

///////////////////////////////////////////////////
///         Low Level Networking
///////////////////////////////////////////////////

#include <stdio.h>
#include <errno.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>	// for sockaddr_in, INADDR_ANY
#include <arpa/inet.h>	// for inet_pton(...)
#include <unistd.h>		// for close(...)
#include <netinet/tcp.h>
#include  <netdb.h>
#include <string.h>

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

////////////////////////////////////////////////////////////////////////////////////

TcpSocket::TcpSocket () :
    mSocket ( -1 )
{
    pthread_mutex_init(&mClientListLock, NULL);
}

TcpSocket::~TcpSocket ()
{
}

void TcpSocket::openServer(int port)
{
    if( mSocket != -1 ) return;
    if ( mMode != CommunicationPolicy::None ) return;
    if ( mMode == CommunicationPolicy::Erroneous ) return;
    int m_sock;
    int on;
    struct sockaddr_in m_addr;
    int bind_return;
    int listen_return;
    m_sock = socket( PF_INET, SOCK_STREAM, 0);
    if( m_sock == -1 )
        printf("Communication::srvOpenPort : error in server socket creation.\n");
    on = 1;
    if( setsockopt( m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*) &on, sizeof(on)) == -1 )
        printf( "Communication::srvOpenPort : error in server socket configuration.\n");
    if( setsockopt( m_sock, IPPROTO_TCP, TCP_NODELAY, (char *) &on, sizeof(int)) < 0 )
        printf( "%s : no_delay impossible.\n", __FUNCTION__);
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = INADDR_ANY;
    m_addr.sin_port = htons ( port );
    bind_return = bind( m_sock, (struct sockaddr *) &m_addr, sizeof(m_addr));
    if( bind_return == -1 )
        printf("Communication::srvOpenPort : error in server socket binding.\n" );
    listen_return = listen( m_sock, 5);
    if( listen_return == -1 )
        printf("Communication::srvOpenPort : error in server socket listen.\n" );
    mSocket = m_sock;
    mMode = mSocket != -1 ? CommunicationPolicy::Server : CommunicationPolicy::None;
}

QUuid TcpSocket::acceptConnectionFromClient()
{
    QUuid fake;
    if( mSocket == -1 ) return fake;
    if ( mMode != CommunicationPolicy::Server ) return fake;
    if ( mMode == CommunicationPolicy::Erroneous ) return fake;
    struct sockaddr_in m_addr;
    int addr_length;
    addr_length = sizeof(m_addr);
    int nv_sock = accept( mSocket, (struct sockaddr *) &m_addr, (socklen_t *) &addr_length);
    if ( nv_sock == -1 ) {
        return fake;
    }
    TcpSocket* tmp = new TcpSocket;
    tmp->mSocket = nv_sock;
    tmp->mMode = CommunicationPolicy::EndPoint;
    QUuid uuid = QUuid::createUuid();
    pthread_mutex_lock (&mClientListLock);
    mEndPoints [ uuid ] = tmp;
    pthread_mutex_unlock (&mClientListLock);
    return uuid;
}

void TcpSocket::connectToServer( QString host , unsigned int port)
{
    if( mSocket != -1 ) return;
    if ( mMode != CommunicationPolicy::None ) return;
    if ( mMode == CommunicationPolicy::Erroneous ) return;
    int m_sock = -1;
    struct sockaddr_in m_addr;
    int status;
    int on;
    struct hostent *hostent_get_host;
    hostent_get_host = gethostbyname(host.toStdString().c_str());
    m_sock = socket( PF_INET, SOCK_STREAM, 0);
    if( m_sock == -1 )
    {
        printf("cltConnectToServer : error in client socket creation.\n");
    }
    else {
        on = 1;
        if( setsockopt( m_sock, IPPROTO_TCP, TCP_NODELAY, (char *) &on, sizeof(int)) < 0 )
            printf( "%s : no_delay impossible.\n", __FUNCTION__);
        m_addr.sin_family = AF_INET;
        m_addr.sin_port = htons(port);
        memcpy(&(m_addr.sin_addr.s_addr), hostent_get_host->h_addr, hostent_get_host->h_length);
        status = connect( m_sock, (struct sockaddr *) &m_addr, sizeof(m_addr));
        if( status != 0 )
        {
            close(m_sock);
            m_sock = -1;
        }
    }
    mSocket = m_sock;
    mMode = mSocket != -1 ? CommunicationPolicy::Client : CommunicationPolicy::None;
}

void TcpSocket::closeConnection()
{
    if( mSocket == -1 ) return;
    if ( mMode == CommunicationPolicy::None ) return;
    if ( mMode == CommunicationPolicy::Server ) {
    }
    close(mSocket);
    mSocket = -1;
    mMode = CommunicationPolicy::None;
}

int TcpSocket::sendData( const unsigned char* message, const unsigned int length)
{
    if( mSocket == -1 ) return -1;
    if ( mMode == CommunicationPolicy::None ) return -1;
    if ( mMode == CommunicationPolicy::Erroneous ) return -1;
    if ( mMode == CommunicationPolicy::Server ) {
        int result = length;
        QMap < QUuid, CommunicationPolicy* > endpoints;
        pthread_mutex_lock (&mClientListLock);
        foreach ( QUuid client, mEndPoints.keys() ) endpoints [ client ] = mEndPoints [ client ];
        pthread_mutex_unlock (&mClientListLock);
        foreach ( QUuid client, endpoints.keys() ) {
            if ( endpoints[client]->sendData(message,length) != length ) {
                result = -1;
            }
        }
        return result;
    }
    else {
        unsigned int total_size = 0;
        int current_size;
        do
        {
            current_size = send( mSocket, &(message[total_size]), length-total_size, MSG_NOSIGNAL);
            if( current_size <= 0 ) {
                printf("sendData : could not write to socket.\n");
                mMode = CommunicationPolicy::Erroneous;
                return -1;
            }
            total_size += current_size;
        }
        while( total_size < length );
        return total_size;
    }
}

bool TcpSocket::dataAvailable ( QUuid client )
{
    if( mSocket == -1 ) return false;
    if ( mMode != CommunicationPolicy::Server ) return false;
    if ( mMode == CommunicationPolicy::Erroneous ) return false;
    if ( mEndPoints.find ( client ) == mEndPoints.end () ) return false;
    return mEndPoints [ client ]->dataAvailable();
}

bool TcpSocket::dataAvailable ()
{
    long _lWaitTimeMicroseconds = 50;
    if( mSocket == -1 ) return false;
    if ( mMode == CommunicationPolicy::None ) return false;
    if ( mMode == CommunicationPolicy::Erroneous ) return false;
    if ( mMode == CommunicationPolicy::Server ) return false;
    int iSelectReturn = 0;  // Number of sockets meeting the criteria given to select()
    timeval timeToWait;
    int fd_max = -1;          // Max socket descriptor to limit search plus one.
    fd_set readSetOfSockets;  // Bitset representing the socket we want to read
                              // 32-bit mask representing 0-31 descriptors where each
                              // bit reflects the socket descriptor based on its bit position.
    timeToWait.tv_sec  = 0;
    timeToWait.tv_usec = _lWaitTimeMicroseconds;
    FD_ZERO(&readSetOfSockets);
    FD_SET(mSocket, &readSetOfSockets);
    if(mSocket > fd_max)
    {
       fd_max = mSocket;
    }
    iSelectReturn = select(fd_max + 1, &readSetOfSockets, (fd_set*) 0, (fd_set*) 0, &timeToWait);
    // iSelectReturn -1: ERROR, 0: no data, >0: Number of descriptors found which pass test given to select()
    if ( iSelectReturn == 0 )  // Not ready to read. No valid descriptors
    {
        return false;
    }
    else if ( iSelectReturn < 0 )  // Handle error
    {
        mMode = CommunicationPolicy::Erroneous;
        return false;
    }
    // Got here because iSelectReturn > 0 thus data available on at least one descriptor
    // Is our socket in the return list of readable sockets
    if ( FD_ISSET(mSocket, &readSetOfSockets) )
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;
}

int TcpSocket::receiveData( unsigned char* message, const unsigned int length)
{
    if( mSocket == -1 ) return -1;
    if ( mMode == CommunicationPolicy::None ) return -1;
    if ( mMode == CommunicationPolicy::Erroneous ) return -1;
    if ( mMode == CommunicationPolicy::Server ) return -1;
    long long total_size = 0;
    long long current_size;
    do
    {
        current_size = recv( mSocket, &(message[total_size]), length-total_size, 0);
        if( current_size <= 0 ) {
            printf("recvData : could not read from socket.\n");
            mMode = CommunicationPolicy::Erroneous;
            return -1;
        }
        total_size += current_size;
    }
    while( total_size < length );

    return total_size;
}

int TcpSocket::sendData( QUuid client, const unsigned char* message, const unsigned int length)
{
    if( mSocket == -1 ) return -1;
    if ( mMode != CommunicationPolicy::Server ) return -1;
    if ( mMode == CommunicationPolicy::Erroneous ) return -1;
    if ( mEndPoints.find ( client ) == mEndPoints.end () ) return -1;
    int total = mEndPoints [ client ]->sendData ( message, length );
    return total;
}

int TcpSocket::receiveData( QUuid client, unsigned char* message, unsigned int length)
{
    if( mSocket == -1 ) return -1;
    if ( mMode != CommunicationPolicy::Server ) return -1;
    if ( mEndPoints.find ( client ) == mEndPoints.end () ) return -1;
    int result = length;
    if ( mEndPoints[client]->receiveData(message,length) != length ) {
        result = -1;
    }
    return result;
}

bool TcpSocket::isValid ()
{
    return ( mSocket != -1 ) && ( mMode != CommunicationPolicy::None ) && ( mMode != CommunicationPolicy::Erroneous );
}

void TcpSocket::cleanUp(DisconnectedCallback* cbDisconnect)
{
    pthread_mutex_lock (&mClientListLock);
    foreach ( QUuid client, mEndPoints.keys() ) {
        if ( ! mEndPoints[client]->isValid() ) {
            if ( cbDisconnect ) (*cbDisconnect) ( client );
            mEndPoints[client]->closeConnection();
            mEndPoints.remove(client);
        }
    }
    pthread_mutex_unlock (&mClientListLock);
}
