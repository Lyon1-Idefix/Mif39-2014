#ifndef COMMUNICATIONPOLICY_H
#define COMMUNICATIONPOLICY_H

#include "Utility/utility.hpp"

typedef void (DisconnectedCallback) ( QUuid client );

class CommunicationPolicy
{
public:
    virtual ~CommunicationPolicy ();
    //
    // Server dedicated methods
    //
    virtual void openServer(int port) = 0;
    virtual QUuid acceptConnectionFromClient() = 0;
    virtual int sendData( QUuid client, const unsigned char* message, const unsigned int length) = 0;
    virtual int receiveData( QUuid client, unsigned char* message, const unsigned int length) = 0;
    virtual bool dataAvailable (QUuid client) = 0;
    inline bool hasEndPoint () { return mEndPoints.size () > 0; }
    virtual void cleanUp (DisconnectedCallback* cbDisconnect) = 0;
    //
    // Client dedicated methods
    //
    virtual void connectToServer( QString host , unsigned int port) = 0;
    virtual int receiveData( unsigned char* message, const unsigned int length) = 0;
    //
    // Client/Server methods
    //
    virtual void closeConnection() = 0;
    virtual int sendData( const unsigned char* message, const unsigned int length) = 0;
    virtual bool dataAvailable () = 0;
    virtual bool isValid () = 0;
protected:
    CommunicationPolicy ();
    enum {
        None,
        Client,
        Server,
        EndPoint,
        Erroneous
    } mMode;
    QMap < QUuid, CommunicationPolicy* > mEndPoints;
};

#endif // COMMUNICATIONPOLICY_H
