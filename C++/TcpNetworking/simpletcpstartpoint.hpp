#ifndef SIMPLETCPSTARTPOINT_H
#define SIMPLETCPSTARTPOINT_H

#include "Networking/networking.hpp"
#include "TcpNetworking/tcpsocket.hpp"

class SimpleTcpStartPoint :
        virtual public ConnectionStartPoint < TcpSocket >
{
public:
    SimpleTcpStartPoint(ConnectionStartPoint::Options options);
    virtual ~SimpleTcpStartPoint();
    virtual bool start ();
    virtual bool stop ();
    virtual bool isStarted ();
    virtual QUuid listen ();
    virtual bool hasEndPoint ();
    virtual bool send ( const ByteBuffer& buffer );
    virtual bool send ( QUuid client, const ByteBuffer& buffer );
    virtual bool receive ( QUuid client, ByteBuffer& buffer );
};

#endif // SIMPLETCPSTARTPOINT_H
