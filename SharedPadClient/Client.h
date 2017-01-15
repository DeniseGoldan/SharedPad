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
#include "StatusCodesAndDescriptions.h"

namespace spd = spdlog;
using namespace rapidjson;
using namespace std;

class Client
{
public:
    Client();
    static int establishConnection();
    static GenericResponseMessage* login(string username);
    static GenericResponseMessage* pair(string sender, string receiver);
    static GenericResponseMessage* synchronize(string username, string content);
    static GenericResponseMessage* sendRequestToServer(string jsonRequest);

private:
    static sockaddr_in serverConfiguration;
    static const char * ip;
    static const in_port_t port;

    static int readJsonResponseLengthFromServer(int socketFD);
    static char *readJsonResponseFromServer(int socketFD, int jsonRequestLength);
    static bool stringContainsOnlyDigits(char *string);

};

#endif // CLIENT_H
