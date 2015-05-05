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
    mSocket->cleanUp(mStartPointOptions.cbDisconnect);
    return client;
}

bool SimpleTcpStartPoint::hasEndPoint ()
{
    return mSocket->hasEndPoint ();
}

bool SimpleTcpStartPoint::send ( const ByteBuffer& buffer )
{
    mSocket->cleanUp(mStartPointOptions.cbDisconnect);
    return true;
}

bool SimpleTcpStartPoint::send ( QUuid client, const ByteBuffer& buffer )
{
    bool result = true;
    unsigned int lg;
    unsigned int length = buffer.getLength();
    lg = mSocket->sendData( client, (unsigned char*) & length, sizeof ( unsigned int ) );
    if ( lg != sizeof ( unsigned int ) ) {
        result = false;
    } else {
        lg = mSocket->sendData ( client, buffer.getData(), buffer.getLength() );
        if ( lg != buffer.getLength() ) {
            result = false;
        }
    }
    mSocket->cleanUp(mStartPointOptions.cbDisconnect);
    return result;
}

bool SimpleTcpStartPoint::receive ( QUuid client, ByteBuffer& buffer )
{
    bool result = true;
    unsigned int lg;
    unsigned int length;
    lg = mSocket->receiveData( client, (unsigned char*) & length, sizeof ( unsigned int ) );
    //std::cout << "received buffer size : " << length << std::endl;
    if ( lg != sizeof ( unsigned int ) ) {
        result = false;
    } else {
        buffer.reserve( length );
        lg = mSocket->receiveData ( client, buffer.getData(), buffer.getLength() );
        //std::cout << "received buffer : " << lg << std::endl;
        if ( lg != buffer.getLength() ) {
            result = false;
        }
    }
    mSocket->cleanUp(mStartPointOptions.cbDisconnect);
    return result;
}

bool SimpleTcpStartPoint::dataAvailable(QUuid client)
{
    return mSocket->dataAvailable(client);
}
