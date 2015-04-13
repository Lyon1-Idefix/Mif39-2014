
#include "QImageLoader/qimageloader.hpp"
#include "Wavefront/wavefront.hpp"

#include "TcpNetworking/simpletcpendpoint.hpp"

int main ( int argc, char** argv ) {

    FileDescriptor file ( argv[1]);
    SharedResourceList ress = ResourceHolder::Load(file);
    SharedResourcePtr ptr = ress [0];
    ByteBuffer message = ResourceHolder::ToBuffer(ptr);

    SimpleTcpEndPoint::Options options;
    options.serverIP = "127.0.0.1";
    options.connectionPort = 3000;
    SimpleTcpEndPoint client ( options );
    client.open();
    while ( true ) {
        client.send(message); std::cout << "Sent : " << message.getLength() << " bytes" << std::endl;
        client.receive(message); std::cout << "Recv : " << message.getLength() << " bytes" << std::endl;
    }
    client.close ();
    return 0;
}
