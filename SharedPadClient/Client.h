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

#include "JsonResponseParser.h"
#include "JsonRequestGenerator.h"
#include "GenericRequest.h"
#include "GenericResponse.h"
#include "StatusCodesAndDescriptions.h"
#include "SpecializedRequest.h"

namespace spd = spdlog;
using namespace rapidjson;
using namespace std;

class Client
{
public:
    Client();

    static GenericResponse* login(string username);

    static GenericResponse* pair(string sender, string receiver);

    static GenericResponse* sendNews(string username, string content);

    static GenericResponse* sendRequest(string jsonRequest);

    static GenericResponse* getWriteFailedResponse();

    static GenericResponse* getConnectionFailedResponse();

    static GenericResponse* getJsonParsingFailedResponse();

private:
    static sockaddr_in serverConfiguration;
    static const char * ip;
    static const in_port_t port;

    static int readJsonResponseLength(int socketFD);

    static char *readJsonResponse(int socketFD, int jsonRequestLength);

    static bool stringContainsOnlyDigits(char *string);

};

#endif // CLIENT_H
