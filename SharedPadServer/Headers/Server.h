#ifndef SHAREPADSERVER_SERVER_H
#define SHAREPADSERVER_SERVER_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <map>

#include "../include/rapidjson/document.h"
#include "../include/spdlog/spdlog.h"

#include "Server.h"
#include "JsonMessageContainers/GenericResponseMessage.h"
#include "JsonResponseMessageGenerator.h"
#include "JsonRequestMessageParser.h"
#include "ErrorHandler.h"
#include "NamespaceSPP.h"
#include "ClientInformation.h"
#include "User.h"

namespace spd = spdlog;
using namespace rapidjson;
using namespace std;

class Server {

public:
    Server();

    void startListeningSession();

private:

    static sockaddr_in serverConfiguration;
    static const char * ip;
    static const in_port_t port;
    static std::map<std::string, User> *loggedUsers;

    static void *handleClient(void *client);

    static int readJsonRequestLength(const ClientInformation *currentClient);

    static char *readJsonRequestFromClient(const ClientInformation *currentClient, int jsonRequestLength);

    static bool sendResponseToClient(const GenericResponseMessage &response, int clientSocketFD);

    static GenericResponseMessage *executeRequest(ClientInformation *clientInformation, Document *document);

    static GenericResponseMessage *executeLoginRequest(ClientInformation *clientInformation, Document *document);

    static bool stringContainsOnlyDigits(char *string);
};


#endif //SHAREPADSERVER_SERVER_H
