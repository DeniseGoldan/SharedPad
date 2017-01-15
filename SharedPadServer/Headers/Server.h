#ifndef SERVER_H
#define SERVER_H

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
#include "GenericResponse.h"
#include "JsonResponseGenerator.h"
#include "JsonRequestParser.h"
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

    static char *readJsonRequest(const ClientInformation *currentClient, int jsonRequestLength);

    static bool sendResponse(const GenericResponse &response, int clientSocketFD);

    static GenericResponse *executeGenericRequest(ClientInformation *clientInformation, Document *document);

    static GenericResponse *executeLoginRequest(ClientInformation *clientInformation, Document *document);

    static GenericResponse *executeLogoutRequest(ClientInformation *clientInformation, Document *document);

    static GenericResponse *executeQuery(ClientInformation *clientInformation, Document *document);

    static GenericResponse *executePairRequest(ClientInformation *clientInformation, Document *document);

    static GenericResponse *executeUnpairRequest(ClientInformation *clientInformation, Document *document);

    static GenericResponse *executeSendNews(ClientInformation *clientInformation, Document *document);

    static GenericResponse *executeCheckNews(ClientInformation *clientInformation, Document *document);

    static void disconnectInactiveClients();

    static void *handleDisconnecting(void *);

    static bool stringContainsOnlyDigits(char *string);

    static void printLoggedUsers();

    static void printPairs();

    static bool usernameIsPaired(const char* username);

    static std::string getPeerUsername(std::string username);
};


#endif //SERVER_H
