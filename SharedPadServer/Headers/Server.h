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

    static char *readJsonRequest(const ClientInformation *currentClient, int jsonRequestLength);

    static bool sendResponse(const GenericResponse &response, int clientSocketFD);

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

    static sockaddr_in serverConfiguration;
    static const char *ip;
    static const in_port_t port;

    static map<string, User> *loggedUsers;
    static map<string, string> *pairs;

    static GenericResponse *handleIncorrectRequest();

    static void *terminateThread();

    static GenericResponse *getLoginFailedResponse();

    static GenericResponse *getLoginApprovedResponse();

    static GenericResponse *getLogoutApprovedResponse();

    static GenericResponse *getUnknownResponse();

    static GenericResponse *getResponseBasedOnCommand(const Document *document, const char *command);

    static GenericResponse *getUserNotLoggedInResponse();

    static GenericResponse *getHeartBeatApprovedResponse();

    static GenericResponse *getPairAddedResponse();

    static GenericResponse *getAlreadyPairedResponse();

    static GenericResponse *getInvitedYourselfResponse();

    static GenericResponse *getYourAreSingleResponse();

    static GenericResponse *getSentNewsToPeerResponse();
};


#endif //SERVER_H
