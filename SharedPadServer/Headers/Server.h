#ifndef SHAREPADSERVER_SERVER_H
#define SHAREPADSERVER_SERVER_H

#include <netinet/in.h>
#include <string>
#include <map>
#include "ClientInformation.h"
#include "JsonMessageContainers/GenericResponseMessage.h"
#include "../include/rapidjson/document.h"
#include "User.h"

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

    static char *readJsonRequest(const ClientInformation *currentClient, int jsonRequestLength);

    static bool sendResponseToClient(const GenericResponseMessage &response, int clientSocketFD);

    static GenericResponseMessage *executeRequest(ClientInformation *clientInformation, rapidjson::Document *document);

    static GenericResponseMessage *executeLoginRequest(ClientInformation *clientInformation,
                                                       rapidjson::Document *document);

    static bool stringContainsOnlyDigits(char *string);
};


#endif //SHAREPADSERVER_SERVER_H
