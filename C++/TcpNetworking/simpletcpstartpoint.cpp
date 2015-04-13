#include "simpletcpstartpoint.hpp"

///////////////////////////////////////////////////

SimpleTcpStartPoint::SimpleTcpStartPoint(ConnectionStartPoint::Options options) :
    ConnectionStream < TcpSocket > ( options ),
    ConnectionStartPoint < TcpSocket > ( options )
{
}

SimpleTcpStartPoint::~SimpleTcpStartPoint()
{
}

bool SimpleTcpStartPoint::start ()
{
    mSocket->openServer( mStartPointOptions.connectionPort );
    return mSocket->isValid();
}

bool SimpleTcpStartPoint::stop ()
{
    mSocket->closeConnection();
    return ! mSocket->isValid();
}

bool SimpleTcpStartPoint::isStarted ()
{
    return mSocket->isValid();
}

QUuid SimpleTcpStartPoint::listen ()
{
    QUuid client = mSocket->acceptConnectionFromClient();
    return client;
}

bool SimpleTcpStartPoint::hasEndPoint ()
{
    return mSocket->hasEndPoint ();
}

bool SimpleTcpStartPoint::send ( const ByteBuffer& buffer )
{
    return true;
}

bool SimpleTcpStartPoint::send ( QUuid client, const ByteBuffer& buffer )
{
    unsigned int lg;
    unsigned int length = buffer.getLength();
    lg = mSocket->sendData( client, (unsigned char*) & length, sizeof ( unsigned int ) );
    if ( lg != sizeof ( unsigned int ) ) {
        stop ();
        return false;
    }
    lg = mSocket->sendData ( client, buffer.getData(), buffer.getLength() );
    if ( lg != buffer.getLength() ) {
        stop ();
        return false;
    }
    return true;
}

bool SimpleTcpStartPoint::receive ( QUuid client, ByteBuffer& buffer )
{
    unsigned int lg;
    unsigned int length;
    lg = mSocket->receiveData( client, (unsigned char*) & length, sizeof ( unsigned int ) );
    if ( lg != sizeof ( unsigned int ) ) {
        stop ();
        return false;
    }
    buffer.reserve( length );
    lg = mSocket->receiveData ( client, buffer.getData(), buffer.getLength() );
    if ( lg != buffer.getLength() ) {
        stop ();
        return false;
    }
    return true;
}
