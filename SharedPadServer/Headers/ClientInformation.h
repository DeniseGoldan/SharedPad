#ifndef SHAREPADSERVER_CLIENT_H
#define SHAREPADSERVER_CLIENT_H

#include <netinet/in.h>

class ClientInformation {
public:
    ClientInformation();

    sockaddr_in address;
    socklen_t addressLength;
    int clientSocketFD;

};


#endif //SHAREPADSERVER_CLIENT_H
