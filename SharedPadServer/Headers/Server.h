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
#include "SpecializedResponse.h"
#include "JsonResponseGenerator.h"
#include "JsonRequestParser.h"
#include "ErrorHandler.h"
#include "StatusCodesAndDescriptions.h"
#include "ClientInformation.h"
#include "User.h"

namespace spd = spdlog;
using namespace rapidjson;
using namespace std;

class Server
{

public:
    Server();

    void startListeningSession();

private:

    static void *handleClient(void *client);

    // Routines executed by the server in order to satisfy the requests coming from the clients

    static GenericResponse *executeGenericRequest(const Document *document);

    static GenericResponse *executeLoginRequest(const Document *document);

    static GenericResponse *executeLogoutRequest(const Document *document);

    static GenericResponse *executeHeartbeatConfirmation(const Document *document);

    static GenericResponse *executePairRequest(const Document *document);

    static GenericResponse *executeUnpairRequest(const Document *document);

    static GenericResponse *executeSendNewsRequest(const Document *document);

    static GenericResponse *executeCheckNewsRequest(const Document *document);

    // Exchanging messages with the clients

    static int readJsonRequestLength(const ClientInformation *currentClient);

    static char *readJsonRequest(const ClientInformation *currentClient, int jsonLength);

    static bool sendResponse(const GenericResponse &response, int clientSocketFD);

    static int getBytesToReadInCurrentSession(int total);

    static char *getPrefixedJson(const GenericResponse &response);

    // Server's disconnecting service

    static void startDisconnectingService();

    static void *handleDisconnecting(void *);

    // Utility functions

    static bool usernameIsPaired(const char *username);

    static void removePairContainingUsername(const string &username);

    static std::string getPeerUsername(std::string username);

    static bool stringContainsOnlyDigits(char *string);

    // Print functions

    static void printLoggedUsers();

    static void printPairs();

    // Members

    static sockaddr_in configuration;
    static const char *ip;
    static const in_port_t port;

    static map<string, User> *loggedUsers;
    static map<string, string> *pairs;

    // Miscellaneous
    static GenericResponse *getResponseBasedOnCommand(const Document *document, const char *command);

    static GenericResponse *handleIncorrectRequest();

    static void *terminateThread();

};


#endif //SERVER_H
