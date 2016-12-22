#ifndef CLIENT_H
#define CLIENT_H

#include <string.h>
#include <arpa/inet.h>
#include <cstdio>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "GenericRequestMessage.h"
#include "GenericResponseMessage.h"

class Client
{
public:
    Client();
    static int establishConnection();
    static GenericResponseMessage* login(std::string username);
    static GenericResponseMessage * sendRequestToServer(std::string jsonRequest);

private:
    static sockaddr_in serverConfiguration;
    static const char * ip;
    static const in_port_t port;


    static int readJsonResponseLengthFromServer(int socketFD);
    static char *readJsonResponseFromServer(int socketFD, int jsonRequestLength);
    static bool stringContainsOnlyDigits(char *string);

};

#endif // CLIENT_H
