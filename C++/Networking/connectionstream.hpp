#ifndef CONNECTIONSTREAM_H
#define CONNECTIONSTREAM_H

#include "Utility/utility.hpp"
#include "Networking/communicationpolicy.hpp"

template < class SocketPolicy >
class ConnectionStream
{
public:
    class Options {
    public:
        unsigned int connectionPort;
        unsigned int flags;
    };
    //
    virtual ~ConnectionStream() {}
protected:
    ConnectionStream(Options options) :
        mBaseOptions ( options ), mSocket ( new SocketPolicy ) {}
    Options mBaseOptions;
    SocketPolicy* mSocket;
};

template < class SocketPolicy >
class ConnectionEndPoint :
        virtual public ConnectionStream < SocketPolicy >
{
public:
    class Options : public ConnectionStream < SocketPolicy >::Options {
    public:
        QString serverIP;
        unsigned int flags;
    };
    //
    virtual ~ConnectionEndPoint() {}
    virtual bool open () = 0;
    virtual bool close () = 0;
    virtual bool isConnected () = 0;
    virtual bool send ( const ByteBuffer& buffer ) = 0;
    virtual bool receive ( ByteBuffer& buffer ) = 0;
    virtual bool dataAvailable () = 0;
protected:
    ConnectionEndPoint(Options options) :
        ConnectionStream < SocketPolicy > ( options ),
        mEndPointOptions ( options )
    {
    }
    Options mEndPointOptions;
};

template < class SocketPolicy >
class ConnectionStartPoint :
        virtual public ConnectionStream < SocketPolicy >
{
public:
    class Options : public ConnectionStream < SocketPolicy >::Options {
    public:
        unsigned int maximumConnectedClients;
        unsigned int flags;
        DisconnectedCallback* cbDisconnect;
    };
    virtual ~ConnectionStartPoint() {}
    virtual bool start () = 0;
    virtual QUuid listen () = 0;
    virtual bool stop () = 0;
    virtual bool isStarted () = 0;
    virtual bool hasEndPoint () = 0;
    virtual bool send ( const ByteBuffer& buffer ) = 0;
    virtual bool send ( QUuid client, const ByteBuffer& buffer ) = 0;
    virtual bool receive ( QUuid client, ByteBuffer& buffer ) = 0;
    virtual bool dataAvailable ( QUuid client ) = 0;
protected:
    ConnectionStartPoint(Options options) :
        ConnectionStream < SocketPolicy > ( options ),
        mStartPointOptions ( options ) {}
    Options mStartPointOptions;
};

#endif // CONNECTIONSTREAM_H
