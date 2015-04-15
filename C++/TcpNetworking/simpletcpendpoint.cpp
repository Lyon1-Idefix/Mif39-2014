#include "simpletcpendpoint.hpp"

///////////////////////////////////////////////////

SimpleTcpEndPoint::SimpleTcpEndPoint(ConnectionEndPoint::Options options) :
    ConnectionStream < TcpSocket > ( options ),
    ConnectionEndPoint < TcpSocket > ( options )
{
}

SimpleTcpEndPoint::~SimpleTcpEndPoint()
{
}

bool SimpleTcpEndPoint::open ()
{
    mSocket->connectToServer( mEndPointOptions.serverIP , mEndPointOptions.connectionPort );
    return mSocket->isValid();
}

bool SimpleTcpEndPoint::close ()
{
    mSocket->closeConnection();
    return ! mSocket->isValid();
}

bool SimpleTcpEndPoint::isConnected ()
{
    return mSocket->isValid();
}

bool SimpleTcpEndPoint::send ( const ByteBuffer& buffer )
{
    unsigned int lg;
    unsigned int length = buffer.getLength();
    lg = mSocket->sendData( (unsigned char*) & length, sizeof ( unsigned int ) );
    if ( lg != sizeof ( unsigned int ) ) {
        close ();
        return false;
    }
    lg = mSocket->sendData ( buffer.getData(), buffer.getLength() );
    if ( lg != buffer.getLength() ) {
        close ();
        return false;
    }
    return true;
}

bool SimpleTcpEndPoint::receive ( ByteBuffer& buffer )
{
    unsigned int lg;
    unsigned int length;
    lg = mSocket->receiveData( (unsigned char*) & length, sizeof ( unsigned int ) );
    if ( lg != sizeof ( unsigned int ) ) {
        close ();
        return false;
    }
    buffer.reserve( length );
    lg = mSocket->receiveData ( buffer.getData(), buffer.getLength() );
    if ( lg != buffer.getLength() ) {
        close ();
        return false;
    }
    return true;
}

bool SimpleTcpEndPoint::dataAvailable()
{
    return mSocket->dataAvailable();
}

