#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "Networking/networking.hpp"

class TcpSocket :
        virtual public CommunicationPolicy
{
public:
    TcpSocket ();
    virtual ~TcpSocket ();
    //
    virtual void openServer(int port);
    virtual QUuid acceptConnectionFromClient();
    virtual int sendData( QUuid client, const unsigned char* message, const unsigned int length);
    virtual int receiveData( QUuid client, unsigned char* message, const unsigned int length);
    //
    virtual void connectToServer( QString host , unsigned int port);
    virtual int receiveData( unsigned char* message, const unsigned int length);
    //
    virtual void closeConnection();
    virtual int sendData( const unsigned char* message, const unsigned int length);
    virtual bool isValid ();
protected:
    int mSocket;
};

#endif // TCPSOCKET_H
