#include <string.h>
#include <arpa/inet.h>
#include <cstdio>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "../Headers/Server.h"
#include "../Headers/ErrorHandler.h"
#include "../Headers/ClientInformation.h"
#include "../Headers/JsonMessageContainers/LoginResponseMessage.h"
#include "../Headers/JsonResponseMessageGenerator.h"
#include "../Headers/JsonRequestMessageParser.h"
#include "../Headers/NamespaceSPP.h"
#include "../include/rapidjson/document.h"
#include "../Headers/User.h"

const char *Server::ip = "127.0.0.1";
const in_port_t Server::port = 2024;
sockaddr_in Server::serverConfiguration;

std::map<std::string, User> *Server::loggedUsers;

Server::Server()
{
    // Preparing the data structure user by the server
    bzero(&serverConfiguration, sizeof(sockaddr_in));
    serverConfiguration.sin_family = AF_INET;
    inet_aton(ip, &serverConfiguration.sin_addr);
    serverConfiguration.sin_port = htons(port);

    // Initializing the map containing the clients
    loggedUsers = new std::map<std::string, User>();
}

void Server::startListeningSession()
{
    // Creating a socket
    int serverSocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == serverSocketFD)
    {
        ErrorHandler::handleErrorAndExitFailure("Socket creation failed.\n");
    }

    // Setting the REUSEADDR option
    int enable = 1;
    if (-1 == setsockopt(serverSocketFD, SOL_SOCKET, SO_REUSEADDR, &enable, (socklen_t) sizeof(enable)))
    {
        ErrorHandler::handleErrorAndExitFailure("Setting socket option (SO_REUSEADDR) failed.\n");
    }

    // Attaching the socket to the server configuration initialized in the constructor
    if (-1 == bind(serverSocketFD, (const sockaddr *) &serverConfiguration, sizeof(sockaddr)))
    {
        ErrorHandler::handleErrorAndExitFailure("Bind failed.\n");
    }

    // Setting socket backlog to "SOcket MAX CONNections"
    if (-1 == listen(serverSocketFD, SOMAXCONN))
    {
        ErrorHandler::handleErrorAndExitFailure("Listening failed.\n");
    }

    // The loop where the server treats every REQUEST and sends a RESPONSE for each request
    while (1)
    {
        // Creating a new ClientInformation instance
        ClientInformation *currentClient = new ClientInformation();

        // Waiting for and accepting a new client
        currentClient->clientSocketFD =
                accept(serverSocketFD, (sockaddr *) &currentClient->address, &currentClient->addressLength);
        if (-1 == currentClient->clientSocketFD)
        {
            ErrorHandler::handleErrorWithoutExit("ClientInformation acceptance failed.\n");
            continue;
        }

        // Concurrency of the server comes from the usage of threads
        pthread_t threadId;
        if (0 != pthread_create(&threadId, nullptr, Server::handleClient, (void *) currentClient))
        {
            // Send a RESPONSE to the client about the error
            GenericResponseMessage response;
            response.setCode(THREAD_CREATION_FAILURE_CODE);
            response.setCodeDescription("Thread creation failed.\n");
            sendResponseToClient(response, currentClient->clientSocketFD);
            // EXIT_FAILURE
            ErrorHandler::handleErrorAndExitFailure("Thread creation failed.\n");
        }
    }
}

void *Server::handleClient(void *client)
{
    ClientInformation *currentClient = (ClientInformation *) client;

    // Setting socket options SO_RCVTIMEO and SO_SNDTIMEO to set timeouts for socket operations
    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    //http://stackoverflow.com/questions/4181784/how-to-set-socket-timeout-in-c-when-making-multiple-connections

    if (-1 == setsockopt(currentClient->clientSocketFD, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout)))
    {
        ErrorHandler::handleErrorWithoutExit("Setting socket option (SO_RCVTIMEO) failed.\n");
    }

    if (-1 == setsockopt(currentClient->clientSocketFD, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(timeout)))
    {
        ErrorHandler::handleErrorWithoutExit("Setting socket option (SO_SNDTIMEO) failed.\n");
    }

    // Read the request from the client's socket
    int jsonRequestLength = readJsonRequestLength(currentClient);
    char *jsonRequest = readJsonRequest(currentClient, jsonRequestLength);
    rapidjson::Document *jsonDocument = JsonRequestMessageParser::parseJsonMessage(jsonRequest);

    // Execute request and send a response back to the client
    GenericResponseMessage *message = executeRequest(currentClient, jsonDocument);
    sendResponseToClient(*message, currentClient->clientSocketFD);

    free(jsonRequest);
    //free(jsonDocument);

    pthread_detach(pthread_self());
    pthread_exit(nullptr);
}

int Server::readJsonRequestLength(const ClientInformation *currentClient)
{
    // Return -1 if any error encountered

    // Read character after character until '\n' is encountered in order to obtain message length
    int totalBytesRead = 0;
    int count = 0;
    // must have '\0' at the end of the string, this means 2 characters
    // char currentCharacter; was not right
    // http://stackoverflow.com/questions/4834811/strcat-concat-a-char-onto-a-string

    char currentCharacter[2];

    char *prefix = (char *) malloc(sizeof(char) * PREFIX_LENGTH);
    bzero(prefix, sizeof(prefix));

    while (totalBytesRead < PREFIX_LENGTH)
    {
        count = (int) read(currentClient->clientSocketFD, currentCharacter, 1);
        if (-1 == count)
        {
            GenericResponseMessage response;
            response.setCode(READ_FAILURE_CODE);
            response.setCodeDescription(READ_FAILURE);
            sendResponseToClient(response, currentClient->clientSocketFD);
            pthread_detach(pthread_self());
            ErrorHandler::handleErrorWithoutExit("Reading the request failed.\n");
            return -1;
        }
        if (currentCharacter[0] == '\n')
        {
            break;
        }
        else
        {
            currentCharacter[1] ='\0';
        }
        totalBytesRead += count;
        strcat(prefix, currentCharacter);
    }

    if (!stringContainsOnlyDigits(prefix))
    {
        GenericResponseMessage response;
        response.setCode(READ_FAILURE_CODE);
        response.setCodeDescription("The prefix of the request contained invalid characters.\n");
        sendResponseToClient(response, currentClient->clientSocketFD);

        pthread_detach(pthread_self());

        ErrorHandler::handleErrorWithoutExit("The prefix of the request contained invalid characters.\n");
        return -1;
    }

    return atoi(prefix);
}

char *Server::readJsonRequest(const ClientInformation *currentClient, int jsonRequestLength)
{
    if (-1 == jsonRequestLength){
        GenericResponseMessage response;
        response.setCode(READ_FAILURE_CODE);
        response.setCodeDescription("The request length is invalid.\n");
        sendResponseToClient(response, currentClient->clientSocketFD);
        return nullptr;
    }

    char *jsonRequest = (char *) malloc(sizeof(char) * (jsonRequestLength + 1));
    bzero(jsonRequest, sizeof(jsonRequest));

    int totalBytesRead = 0;
    int count = 0;
    int totalBytesLeftToRead = jsonRequestLength;

    while (totalBytesLeftToRead > 0)
    {
        int bytesToReadInCurrentSession;

        (totalBytesLeftToRead < BUFF_SIZE) ?
                bytesToReadInCurrentSession = totalBytesLeftToRead :
                bytesToReadInCurrentSession = BUFF_SIZE;

        count = (int) read(currentClient->clientSocketFD, jsonRequest + totalBytesRead,
                           (size_t) bytesToReadInCurrentSession);
        if (-1 == count)
        {
            GenericResponseMessage response;
            response.setCode(READ_FAILURE_CODE);
            response.setCodeDescription(READ_FAILURE);
            sendResponseToClient(response, currentClient->clientSocketFD);
            pthread_detach(pthread_self());
            ErrorHandler::handleErrorWithoutExit("Reading request from client failed.\n");
            return nullptr;
        }
        if (count == 0)
        {
            break;
        }
        totalBytesRead += count;
        totalBytesLeftToRead -= count;
    }

    if (jsonRequest == nullptr)
    {
        GenericResponseMessage response;
        response.setCode(READ_FAILURE_CODE);
        response.setCodeDescription(READ_FAILURE);
        sendResponseToClient(response, currentClient->clientSocketFD);
        pthread_detach(pthread_self());
        ErrorHandler::handleErrorWithoutExit("Reading request from client failed.\n");
        return nullptr;
    }

    printf("%s\n",jsonRequest);
    return jsonRequest;
}

bool Server::sendResponseToClient(const GenericResponseMessage &response, int clientSocketFD)
{
    // Turn the generic response into a string
    std::string jsonResponse = JsonResponseMessageGenerator::getJsonBasicResponseMessage(response);

    // Must add a prefix with the length of the response
    int length = (int) jsonResponse.length();
    char *prefixedJsonResponse = (char *) malloc((size_t) (PREFIX_LENGTH + length + 1));
    bzero(prefixedJsonResponse, sizeof(prefixedJsonResponse));
    sprintf(prefixedJsonResponse, "%d\n%s", length, jsonResponse.c_str());

    // Write response into socket
    int totalBytesLeftToSend = 1 + (int) strlen(prefixedJsonResponse);
    int totalBytesSent = 0;
    int count = 0;

    while (totalBytesLeftToSend > 0)
    {
        count = (int) write(clientSocketFD, prefixedJsonResponse + totalBytesSent, BUFF_SIZE);
        if (-1 == count)
        {
            ErrorHandler::handleErrorWithoutExit("Writing message to client failed.\n");
            free(prefixedJsonResponse);
            return false;
        } else if (0 == count)
        {
            free(prefixedJsonResponse);
            return true;
        }
        totalBytesSent += count;
        totalBytesLeftToSend -= count;
    }

    free(prefixedJsonResponse);
    return true;
    //http://codereview.stackexchange.com/questions/27213/sending-large-data-package-with-tcp-winsock
    //http://stackoverflow.com/questions/1577825/unix-sockets-how-to-send-really-big-data-with-one-send-call
}

GenericResponseMessage *Server::executeRequest(ClientInformation *currentClient, rapidjson::Document *document)
{
    if (document == nullptr)
    {
        GenericResponseMessage response;
        response.setCode(READ_FAILURE_CODE);
        response.setCodeDescription("Parsing the request failed.\n");
        sendResponseToClient(response, currentClient->clientSocketFD);
        pthread_detach(pthread_self());
        ErrorHandler::handleErrorWithoutExit("Parsing the request failed.\n");
    }

    //This basic response will change if the request will be properly executed
    GenericResponseMessage *response = new GenericResponseMessage();
    response->setCode(UNKNOWN_CODE);
    response->setCodeDescription(UNKNOWN);

    // Login request
    if (0 == strcmp(LOGIN, document->FindMember(COMMAND)->value.GetString()))
    {
        response = executeLoginRequest(currentClient, document);
    }

    return response;
}

GenericResponseMessage *Server::executeLoginRequest(ClientInformation *clientInformation, rapidjson::Document *document)
{
    GenericResponseMessage *response = new GenericResponseMessage();
    std::string username = document->FindMember(ARGUMENTS)->value[USERNAME].GetString();
    if (loggedUsers->find(username) == loggedUsers->cend())
    {
        // Add new user to the loggedUsers map
        User *newUser = new User();
        newUser->setAdress(clientInformation->address);
        newUser->setGroup(-1);
        loggedUsers->insert(std::pair<std::string, User>(username, *newUser));
        // Send a "approved login" response back to the client
        response->setCode(LOGIN_APPROVED_CODE);
        response->setCodeDescription(LOGIN_APPROVED);
        printf("%s","Login approved.\n");
    }
    else
    {
        // Username is already registered, send a "login failed" response back to the client
        response->setCode(LOGIN_FAILED_CODE);
        response->setCodeDescription(LOGIN_FAILED);
        printf("%s","Login NOT approved.\n");
    }

    return response;
}

bool Server::stringContainsOnlyDigits(char *string)
{
    int length = strlen(string);
    for (int i = 0; i < length; i++)
    {
        if (string[i] < '0' || string[i] > '9')
        {
            return false;
        }
    }
    return true;
}
