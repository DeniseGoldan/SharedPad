#ifndef SHAREDPADSERVER_SERVER_H
#define SHAREDPADSERVER_SERVER_H

#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <map>

#include "../include/rapidjson/document.h"
#include "../include/spdlog/spdlog.h"

#include "Server.h"
#include "GenericResponseMessage.h"
#include "JsonResponseMessageGenerator.h"
#include "JsonRequestMessageParser.h"
#include "ErrorHandler.h"
#include "StatusCodesAndDescriptions.h"
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
    static map<string, User> *loggedUsers;
    static map<string, string> *pairs;

    static void *handleClient(void *client);

    static int readJsonRequestLength(const ClientInformation *currentClient);

    static char *readJsonRequestFromClient(const ClientInformation *currentClient, int jsonRequestLength);

    static bool sendResponseToClient(const GenericResponseMessage &response, int clientSocketFD);

    static GenericResponseMessage *executeRequest(ClientInformation *clientInformation, Document *document);

    static GenericResponseMessage *executeLoginRequest(ClientInformation *clientInformation, Document *document);

    static GenericResponseMessage *executeLogoutRequest(ClientInformation *clientInformation, Document *document);

    static GenericResponseMessage *executeQuery(ClientInformation *clientInformation, Document *document);

    static GenericResponseMessage *executePairRequest(ClientInformation *clientInformation, Document *document);

    static void disconnectInactiveClients();

    static void *handleDisconnecting(void *);

    static bool stringContainsOnlyDigits(char *string);

    static void printLoggedUsers();

    static void printPairs();

    static bool usernameIsPaired(const char* username);
};


#endif //SHAREDPADSERVER_SERVER_H
