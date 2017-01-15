#include "../Headers/Server.h"

sockaddr_in Server::configuration;
const char *Server::ip = "127.0.0.1";
const in_port_t Server::port = 2024;
map<string, User> *Server::loggedUsers;
map<string, string> *Server::pairs;

auto startListeningSession_logger = spd::stdout_color_mt("startListeningSession_logger");
auto handleClient_logger = spd::stdout_color_mt("handleClient_logger");
auto readJsonRequestLength_logger = spd::stdout_color_mt("readJsonRequestLength_logger");
auto readJsonRequestFromClient_logger = spd::stdout_color_mt("readJsonRequestFromClient_logger");
auto sendResponseToClient_logger = spd::stdout_color_mt("sendResponseToClient_logger");
auto executeRequest_logger = spd::stdout_color_mt("executeRequest_logger");
auto handleDisconnecting_logger = spd::stdout_color_mt("handleDisconnecting_logger");
auto heartbeat_logger = spd::stdout_color_mt("heartbeat_logger");
auto pairRequest_logger = spd::stdout_color_mt("pairRequest_logger");

Server::Server()
{
    memset(&configuration, 0, sizeof(struct sockaddr_in));
    configuration.sin_family = AF_INET;
    inet_aton(ip, &configuration.sin_addr);
    configuration.sin_port = htons(port);

    loggedUsers = new map<string, User>();
    pairs = new map<string, string>();

    startDisconnectingService();
}

void Server::startListeningSession()
{
    int serverSocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == serverSocketFD)
    {
        ErrorHandler::exitFailure("CRITICAL - Socket creation failed.\n");
    }
    int enable = 1;
    if (-1 == setsockopt(serverSocketFD, SOL_SOCKET, SO_REUSEADDR, &enable, (socklen_t) sizeof(enable)))
    {
        ErrorHandler::exitFailure("CRITICAL - Setting SO_REUSEADDR failed.\n");
    }
    if (-1 == bind(serverSocketFD, (const sockaddr *) &configuration, sizeof(sockaddr)))
    {
        ErrorHandler::exitFailure("CRITICAL - Binding failed.\n");
    }
    if (-1 == listen(serverSocketFD, SOMAXCONN))
    {
        ErrorHandler::exitFailure("CRITICAL - Listening failed.\n");
    }
    while (true)
    {
        ClientInformation *currentClient = new ClientInformation();
        currentClient->clientSocketFD = accept(serverSocketFD,
                                               (sockaddr *) &currentClient->address, &currentClient->addressLength);

        if (-1 == currentClient->clientSocketFD)
        {
            startListeningSession_logger->warn("Accepting client failed.");
            continue;
        }
        pthread_t threadId;
        if (0 != pthread_create(&threadId, nullptr, Server::handleClient, (void *) currentClient))
        {
            startListeningSession_logger->warn("Thread creation failed.");
            continue;
        }
    }
}

/**
 * handleClient
 *
 * Each request coming from the client has the following form:"<lenght>\n<content>", where <length> is the length of the
 * <content>, and <content> is a json that consists of a command (e.g.: "login", "heartbeat") and its arguments
 * (e.g: "username", "sender").
 * In order to process the request, the lenght must be extracted, and based on the length, a number of bits will
 * be read from the socket, representing the json. The json will be parsed and, the command with its arguments will be
 * extracted. The command will be passed to executeGenericRequest, a function that returns the response from the server,
 * after executing the command.
 **/
void *Server::handleClient(void *client)
{
    ClientInformation *currentClient = (ClientInformation *) client;

    handleClient_logger->info("Reading the request length.");
    int jsonRequestLength = readJsonRequestLength(currentClient);
    if (jsonRequestLength == -1)
    {
        handleClient_logger->warn("jsonRequestLength == -1");
        return terminateThread();
    }

    char *jsonRequest = readJsonRequest(currentClient, jsonRequestLength);
    handleClient_logger->info("Reading the request. The request is: ");
    handleClient_logger->info(jsonRequest);
    if (jsonRequest == nullptr)
    {
        handleClient_logger->warn("jsonRequest == nullptr");
        free(jsonRequest);
        return terminateThread();
    }

    handleClient_logger->info("Parsing the request.");
    Document *jsonDocument = JsonRequestParser::parseJson(jsonRequest);
    if (jsonDocument == nullptr)
    {
        handleClient_logger->warn("jsonDocument == nullptr");
        free(jsonRequest);
        free(jsonDocument);
        return terminateThread();
    }

    handleClient_logger->info("Executing the request.");
    GenericResponse *message = executeGenericRequest(jsonDocument);

    handleClient_logger->info("Sending a response back. The response is: ");
    handleClient_logger->info(message->getCodeDescription());
    sendResponse(*message, currentClient->clientSocketFD);

    return terminateThread();
}

/**
 * Returns the response from the server, after executing the command specified in the document.
 * */
GenericResponse *Server::executeGenericRequest(const Document *document)
{
    if (document == nullptr)
    {
        executeRequest_logger->warn("Parsing the request failed.");
        return handleIncorrectRequest();
    }
    executeRequest_logger->warn("Determining the actual command.");
    char *command = (char *) malloc(500 * sizeof(char));
    strcpy(command, document->FindMember(COMMAND)->value.GetString());
    return getResponseBasedOnCommand(document, command);
}

GenericResponse *Server::getResponseBasedOnCommand(const Document *document, const char *command)
{
    if (0 == strcmp(LOGIN, command)) { return executeLoginRequest(document); }
    if (0 == strcmp(LOGOUT, command)) { return executeLogoutRequest(document); }
    if (0 == strcmp(HEARTBEAT, command)) { return executeHeartbeatConfirmation(document); }
    if (0 == strcmp(PAIR, command)) { return executePairRequest(document); }
    if (0 == strcmp(UNPAIR, command)) { return executeUnpairRequest(document); }
    if (0 == strcmp(SEND_NEWS, command)) { return executeSendNewsRequest(document); }
    if (0 == strcmp(CHECK_NEWS, command)) { return executeCheckNewsRequest(document); }
    return SpecializedResponse::getUnknownResponse();
}

/**
 * LOGIN
 *
 * Adds username key to loggedUsers map, along with its own User instance, if username is not already taken
 * and sends a "login approved"  * response back to the client. If username is already registered, sends a
 * "login failed" response back to the client.
**/
GenericResponse *Server::executeLoginRequest(const Document *document)
{
    string username = document->FindMember(ARGUMENTS)->value[USERNAME].GetString();
    if (loggedUsers->find(username) == loggedUsers->cend())
    {
        User *newUserInformation = new User();
        loggedUsers->insert(pair<string, User>(username, *newUserInformation));
        return SpecializedResponse::getLoginApprovedResponse();
    }
    else
    {
        return SpecializedResponse::getLoginFailedResponse();
    }
}

/**
 * LOGOUT
 *
 * Removes username from loggedUsers map and removes pair if username is in a pair and returns a response.
**/

GenericResponse *Server::executeLogoutRequest(const Document *document)
{
    string username = document->FindMember(ARGUMENTS)->value[USERNAME].GetString();
    loggedUsers->erase(username);
    removePairContainingUsername(username);
    return SpecializedResponse::getLogoutApprovedResponse();
}

/**
 * HEARTBEAT
 *
 * Each clients has a worker that sends a request to the server to update its last connection check time, so that the
 * server can keep track of the client's activity. After receiving such a message, the server updates the field
 * "last check" from the corresponding User instance. If the clients hasn't sent any heartbeat confirmations after
 * a certain amount of time (10 seconds), the disconnecting inactive clients service will remove the username from
 * the loggedUsers map along with its pair from pairs map, considering that the client is inactive.
 * */
GenericResponse *Server::executeHeartbeatConfirmation(const Document *document)
{
    string username = document->FindMember(ARGUMENTS)->value[USERNAME].GetString();
    if (loggedUsers->find(username) == loggedUsers->cend())
    {
        return SpecializedResponse::getUserNotLoggedInResponse();
    }
    else
    {
        timeval now; gettimeofday(&now, NULL);
        loggedUsers->at(username).updateLastCheck(now);

        heartbeat_logger->warn("heartbeat");
        heartbeat_logger->warn(username);

        return SpecializedResponse::getHeartBeatApprovedResponse();
    }
}

/**
 * PAIR
 *
 * Pairs the sender and receiver only if they are both logged and if they are not already paired.
 * */
GenericResponse *Server::executePairRequest(const Document *document)
{
    string sender = document->FindMember(ARGUMENTS)->value[SENDER].GetString();
    string receiver = document->FindMember(ARGUMENTS)->value[RECEIVER].GetString();

    if (loggedUsers->find(sender) != loggedUsers->cend())
    {
        pairRequest_logger->info("Sender is logged in");
        if (loggedUsers->find(receiver) != loggedUsers->cend())
        {
            pairRequest_logger->info("Receiver is logged in");
            if (0 == sender.compare(receiver))
            {
                pairRequest_logger->warn("Sender is the same as receiver");
                return SpecializedResponse::getInvitedYourselfResponse();
            }
            else
            {
                pairRequest_logger->info("Sender is different from receiver");
                if (usernameIsPaired(sender.c_str()) || usernameIsPaired(receiver.c_str()))
                {
                    pairRequest_logger->warn("Sender or receiver already paired");
                    return SpecializedResponse::getAlreadyPairedResponse();
                }
                else
                {
                    pairRequest_logger->info("Sender and receiver have not been paired yet");
                    if (!usernameIsPaired(sender.c_str()) && !usernameIsPaired(receiver.c_str()))
                    {
                        pairRequest_logger->info("New pair added");
                        pairs->insert(pair<string, string>(sender, receiver));
                        return SpecializedResponse::getPairAddedResponse();
                    }
                }
            }
        }
        else
        {
            pairRequest_logger->info("receiver not logged in");
            return SpecializedResponse::getUserNotLoggedInResponse();
        }
    }
    else
    {
        pairRequest_logger->info("sender not logged in");
        return SpecializedResponse::getUserNotLoggedInResponse();
    }
}

/**
 * UNPAIR
 *
 * Eliminates pair containing username and returns a reponse back to the client.
 * */
GenericResponse *Server::executeUnpairRequest(const Document *document)
{
    string username = document->FindMember(ARGUMENTS)->value[USERNAME].GetString();
    removePairContainingUsername(username);
    return SpecializedResponse::getYourAreSingleResponse();
}

/**
 * SEND_NEWS
 *
 * Each client must update its peer when the document suffers any alterations (insertion/ deletion).
 * */
GenericResponse *Server::executeSendNewsRequest(const Document *document)
{
    string username = document->FindMember(ARGUMENTS)->value[USERNAME].GetString();
    string content = document->FindMember(ARGUMENTS)->value[CONTENT].GetString();
    if (!usernameIsPaired(username.c_str()))
    {
        return SpecializedResponse::getYourAreSingleResponse();
    }
    else
    {
        string peerUsername = getPeerUsername(username);
        loggedUsers->at(peerUsername).setHasFileContentFromPeer(true);
        loggedUsers->at(peerUsername).setPeerFileContent(content);
        return SpecializedResponse::getSentNewsToPeerResponse();
    }
}

/**
 * CHECK_NEWS
 *
 * Each client has a worker that constantly checks if its corresponding User instance suffered any changes
 * (e.g.: its peer inserted/deleted a character). It also checks if there are any news regarding the pair map.
 * */
GenericResponse *Server::executeCheckNewsRequest(const Document *document)
{
    GenericResponse *response = new GenericResponse();
    string username = document->FindMember(ARGUMENTS)->value[USERNAME].GetString();
    if (!loggedUsers->at(username).doesHaveFileContentFromPeer())
    {
        if (usernameIsPaired(username.c_str()))
        {
            response->setCode(PAIR_ADDED_CODE);
            response->setCodeDescription(getPeerUsername(username));
            return response;
        }
        else
        {
            response->setCode(PAIR_ADDED_CODE);
            response->setCodeDescription("...you do not have a pair");
            return response;
        }
    }
    else
    {
        response->setCode(HAD_NEWS_CODE);
        response->setCodeDescription(loggedUsers->at(username).getPeerFileContent());
        loggedUsers->at(username).setHasFileContentFromPeer(false);
        loggedUsers->at(username).setPeerFileContent("");
        return response;
    }
}

/**
 * Extracts and returns an integer that represents the <lenght> of the request.
 * */
int Server::readJsonRequestLength(const ClientInformation *currentClient)
{
    char *prefix = (char *) malloc(sizeof(char) * PREFIX_LENGTH);
    memset(prefix, 0, sizeof(char) * PREFIX_LENGTH);

    char currentCharacter[2];
    int totalBytesRead = 0, count = 0;

    while (totalBytesRead < PREFIX_LENGTH)
    {
        count = (int) read(currentClient->clientSocketFD, currentCharacter, 1);
        if (-1 == count)
        {
            pthread_detach(pthread_self());
            readJsonRequestLength_logger->warn("Reading the request failed.\n");
            return -1;
        }
        if (currentCharacter[0] == '\n') { break; }
        else { currentCharacter[1] = '\0'; }
        strcat(prefix, currentCharacter);
        totalBytesRead += count;
    }

    if (!stringContainsOnlyDigits(prefix))
    {
        pthread_detach(pthread_self());
        readJsonRequestLength_logger->warn("The prefix of the request contained invalid characters.");
        return -1;
    }
    return atoi(prefix);
}

/**
 * Extracts and returns the <content> of the request based on the length obtained from readJsonRequestLength function.
 * */
char *Server::readJsonRequest(const ClientInformation *currentClient, int jsonRequestLength)
{
    if (-1 == jsonRequestLength)
    {
        readJsonRequestFromClient_logger->warn("The length of the request is invalid.");
        return nullptr;
    }

    int totalBytesRead = 0, count = 0, totalBytesLeftToRead = jsonRequestLength + 1;
    char *jsonResponse = (char *) malloc(sizeof(char) * (jsonRequestLength + 2));

    while (totalBytesLeftToRead > 0)
    {
        int bytesToReadInCurrentSession;

        (totalBytesLeftToRead < BUFF_SIZE) ?
                bytesToReadInCurrentSession = totalBytesLeftToRead :
                bytesToReadInCurrentSession = BUFF_SIZE;

        count = (int) read(currentClient->clientSocketFD, jsonResponse + totalBytesRead,
                           (size_t) bytesToReadInCurrentSession);
        switch (count)
        {
            case -1:
            {
                close(currentClient->clientSocketFD);
                readJsonRequestFromClient_logger->warn("Reading failed: -1 == count");
                return nullptr;
            }
            case 0:
            {
                close(currentClient->clientSocketFD);
                readJsonRequestFromClient_logger->warn("Reading failed: 0 == count");
                return nullptr;
            }
            default:
            {
                totalBytesRead += count;
                totalBytesLeftToRead -= count;
            }
        }
    }
    if (jsonResponse == nullptr)
    {
        readJsonRequestFromClient_logger->warn("Reading failed.");
        return nullptr;
    }
    return jsonResponse;
}

/**
 * Creates a length prefixed json, "<length>\n<content>", where the <length> represents the length of the <content>
 * is a json that consists of a code, a code description and other response specific fields, that depend on the command.
 * */
bool Server::sendResponse(const GenericResponse &response, int clientSocketFD)
{
    string jsonResponse = JsonResponseGenerator::getJsonResponse(response);
    int length = (int) jsonResponse.length();

    char *prefixedJsonResponse = (char *) malloc(sizeof(char) * (PREFIX_LENGTH + length + 1));
    memset(prefixedJsonResponse, 0, sizeof(char) *(PREFIX_LENGTH + length + 1));
    sprintf(prefixedJsonResponse, "%d\n%s", length, jsonResponse.c_str());

    sendResponseToClient_logger->info("Preparing to write the response.");
    int totalBytesLeftToSend = 1 + (int) strlen(prefixedJsonResponse);
    int totalBytesSent = 0, count = 0;

    while (totalBytesLeftToSend > 0)
    {
        count = (int) write(clientSocketFD, prefixedJsonResponse + totalBytesSent, BUFF_SIZE);
        switch (count)
        {
            case -1:
            {
                sendResponseToClient_logger->warn("Writing response failed: -1 == count");
                free(prefixedJsonResponse);
                return false;
            }
            case 0:
            {
                sendResponseToClient_logger->warn("Writing response failed: 0 == count");
                free(prefixedJsonResponse);
                return false;
            }
            default:
            {
                totalBytesSent += count;
                totalBytesLeftToSend -= count;
            }
        }
    }
    free(prefixedJsonResponse);
    return true;
}

/**
 * The server has a disconnecting service that removes inactive clients based on their heartbeat coinfirmations.
 * */
void Server::startDisconnectingService()
{
    pthread_t threadId;
    if (0 != pthread_create(&threadId, nullptr, Server::handleDisconnecting, nullptr))
    {
        ErrorHandler::exitFailure("Thread creation failed.\n");
    }
}

void *Server::handleDisconnecting(void *)
{
    handleDisconnecting_logger->info("Inside function handleDisconnecting.");
    while (true)
    {
        printLoggedUsers();
        printPairs();
        for (auto usersIT = loggedUsers->cbegin(); usersIT != loggedUsers->cend(); usersIT++)
        {
            timeval now; gettimeofday(&now, NULL);
            if (now.tv_sec - usersIT->second.getLastCheck().tv_sec > 10)
            {
                loggedUsers->erase(usersIT);
                removePairContainingUsername(usersIT->first.c_str());
                handleDisconnecting_logger->info("Server erased an inactive client.");
            }
        }
        handleDisconnecting_logger->info("Server's disconnecting service will wait for 10 seconds.");
        sleep(10);
    }
}

/**
 * Print functions
 **/
void Server::printLoggedUsers()
{
    printf("USERS: ");
    for (auto constIterator = loggedUsers->cbegin(); constIterator != loggedUsers->cend(); constIterator++)
    {
        printf("%s | ", (*constIterator).first.c_str());
    }
    printf("\n");
}

void Server::printPairs()
{
    printf("PAIRS: ");
    for (auto constIterator = pairs->cbegin(); constIterator != pairs->cend(); constIterator++)
    {
        printf("%s and %s | ", (*constIterator).first.c_str(), (*constIterator).second.c_str());
    }
    printf("\n");
}

/**
 * Utility functions  that deals with the 2 maps
 * */

bool Server::usernameIsPaired(const char *username)
{
    for (auto constIterator = pairs->cbegin(); constIterator != pairs->cend(); constIterator++)
    {
        if (0 == strcmp((*constIterator).first.c_str(), username))
        {
            return true;
        }
        if (0 == strcmp((*constIterator).second.c_str(), username))
        {
            return true;
        }
    }
    return false;
}

std::string Server::getPeerUsername(std::string username)
{
    for (auto constIterator = pairs->cbegin(); constIterator != pairs->cend(); constIterator++)
    {
        if (0 == strcmp((*constIterator).first.c_str(), username.c_str()))
        {
            return (*constIterator).second;
        }
        if (0 == strcmp((*constIterator).second.c_str(), username.c_str()))
        {
            return (*constIterator).first;
        }
    }
    return "";
}

void Server::removePairContainingUsername(const string &username)
{
    for (auto constIterator = pairs->cbegin(); constIterator != pairs->cend(); constIterator++)
    {
        if (0 == strcmp(username.c_str(), (*constIterator).first.c_str()))
        {
            pairs->erase((*constIterator).first.c_str());
            break;
        }
        if (0 == strcmp(username.c_str(), (*constIterator).second.c_str()))
        {
            pairs->erase((*constIterator).first.c_str());
            break;
        }
    }
}

/**
 * Miscellaneous
 * */
bool Server::stringContainsOnlyDigits(char *string)
{
    char *end;
    long converted = strtol(string, &end, 10);
    if (*end)
    {
        return false;
    }
    return true;
}

void *Server::terminateThread()
{
    pthread_detach(pthread_self());
    pthread_exit(nullptr);
}

GenericResponse *Server::handleIncorrectRequest()
{
    GenericResponse *response = new GenericResponse();
    response->setCode(JSON_PARSING_FAILED_CODE);
    response->setCodeDescription(JSON_PARSING_FAILED);
    return response;
}