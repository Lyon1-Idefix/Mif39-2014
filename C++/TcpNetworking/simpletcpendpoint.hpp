#ifndef SIMPLETCPENDPOINT_H
#define SIMPLETCPENDPOINT_H

#include "Networking/networking.hpp"
#include "TcpNetworking/tcpsocket.hpp"

class SimpleTcpEndPoint :
        virtual public ConnectionEndPoint < TcpSocket >
{
public:
    SimpleTcpEndPoint(ConnectionEndPoint::Options options);
    virtual ~SimpleTcpEndPoint();
    virtual bool open ();
    virtual bool close ();
    virtual bool isConnected ();
    virtual bool send ( const ByteBuffer& buffer );
    virtual bool receive ( ByteBuffer& buffer );
};

#endif // SIMPLETCPENDPOINT_H
