#ifndef CLIENT_H
#define CLIENT_H

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "rapidjson/document.h"
#include "spdlog/spdlog.h"

#include "JsonResponseMessageParser.h"
#include "JsonRequestMessageGenerator.h"
#include "GenericRequestMessage.h"
#include "GenericResponseMessage.h"
#include "NamespaceSPP.h"
#include "ErrorHandler.h"

namespace spd = spdlog;
using namespace rapidjson;

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
