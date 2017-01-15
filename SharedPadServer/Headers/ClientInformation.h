#ifndef SERVER_CLIENT_INFORMATION_H
#define SERVER_CLIENT_INFORMATION_H

#include <netinet/in.h>
#include <string.h>

class ClientInformation
{
public:
    ClientInformation();

    sockaddr_in address;
    socklen_t addressLength;
    int clientSocketFD;
};

#endif //SERVER_CLIENT_INFORMATION_H
