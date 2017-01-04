#ifndef SHAREDPADSERVER_CLIENT_H
#define SHAREDPADSERVER_CLIENT_H

#include <netinet/in.h>
#include <string.h>

class ClientInformation {
public:
    ClientInformation();

    sockaddr_in address;
    socklen_t addressLength;
    int clientSocketFD;
};

#endif //SHAREDPADSERVER_CLIENT_H
