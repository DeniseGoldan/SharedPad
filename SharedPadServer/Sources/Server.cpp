#include "../Headers/Server.h"

const char *Server::ip = "127.0.0.1";
const in_port_t Server::port = 2024;
sockaddr_in Server::serverConfiguration;
map<string, User> *Server::loggedUsers;
map<string, string> *Server::pairs;

auto startListeningSession_logger = spd::stdout_color_mt("startListeningSession_logger");
auto handleClient_logger = spd::stdout_color_mt("handleClient_logger");
auto readJsonRequestLength_logger = spd::stdout_color_mt("readJsonRequestLength_logger");
auto readJsonRequestFromClient_logger = spd::stdout_color_mt("readJsonRequestFromClient_logger");
auto sendResponseToClient_logger = spd::stdout_color_mt("sendResponseToClient_logger");
auto executeRequest_logger = spd::stdout_color_mt("executeRequest_logger");
auto handleDisconnecting_logger = spd::stdout_color_mt("handleDisconnecting_logger");
auto query_logger = spd::stdout_color_mt("query_logger");
auto pairRequest_logger = spd::stdout_color_mt("pairRequest_logger");

//mutex Server::pairsMutex;
//mutex Server::loggedUsersMutex;

Server::Server()
{
    bzero(&serverConfiguration, sizeof(sockaddr_in));
    serverConfiguration.sin_family = AF_INET;
    inet_aton(ip, &serverConfiguration.sin_addr);
    serverConfiguration.sin_port = htons(port);

    loggedUsers = new map<string, User>();
    pairs = new map<string, string>();

    disconnectInactiveClients();
}

void Server::startListeningSession()
{
    int serverSocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == serverSocketFD)
    {
        ErrorHandler::exitFailure("Socket creation failed.\n");
    }

    int enable = 1;
    if (-1 == setsockopt(serverSocketFD, SOL_SOCKET, SO_REUSEADDR, &enable, (socklen_t) sizeof(enable)))
    {
        ErrorHandler::exitFailure("Setting socket option (SO_REUSEADDR) failed.\n");
    }

    if (-1 == bind(serverSocketFD, (const sockaddr *) &serverConfiguration, sizeof(sockaddr)))
    {
        ErrorHandler::exitFailure("Bind failed.\n");
    }

    if (-1 == listen(serverSocketFD, SOMAXCONN))
    {
        ErrorHandler::exitFailure("Listening failed.\n");
    }

    while (1)
    {
        ClientInformation *currentClient = new ClientInformation();
        currentClient->clientSocketFD = accept(serverSocketFD, (sockaddr *) &currentClient->address, &currentClient->addressLength);

        if (-1 == currentClient->clientSocketFD)
        {
            startListeningSession_logger->warn("ClientInformation acceptance failed.");
            continue;
        }

        pthread_t threadId;
        if (0 != pthread_create(&threadId, nullptr, Server::handleClient, (void *) currentClient))
        {
            startListeningSession_logger->critical("Thread creation failed.");
            ErrorHandler::exitFailure("Thread creation failed.\n");
        }
    }
}

void *Server::handleClient(void *client)
{
    handleClient_logger->info("Inside handleClient function.");
    handleClient_logger->info("Setting socket options SO_RCVTIMEO and SO_SNDTIMEO.");
    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    ClientInformation *currentClient = (ClientInformation *) client;
    if (-1 == setsockopt(currentClient->clientSocketFD, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout)))
    {
        handleClient_logger->warn("Setting socket option (SO_RCVTIMEO) failed.");
    }
    if (-1 == setsockopt(currentClient->clientSocketFD, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(timeout)))
    {
        handleClient_logger->warn("Setting socket option (SO_SNDTIMEO) failed.");
    }

    int jsonRequestLength = readJsonRequestLength(currentClient);
    handleClient_logger->info("jsonRequestLength");
    handleClient_logger->info(jsonRequestLength);
    if (jsonRequestLength == -1)
    {
        handleClient_logger->warn("jsonRequestLength == -1");
        pthread_detach(pthread_self());
        pthread_exit(nullptr);
    }
    char *jsonRequest = readJsonRequestFromClient(currentClient, jsonRequestLength);
    handleClient_logger->info("jsonRequest:");
    handleClient_logger->info(jsonRequest);
    if (jsonRequest == nullptr)
    {
        handleClient_logger->warn("jsonRequest == nullptr");
        free(jsonRequest);
        pthread_detach(pthread_self());
        pthread_exit(nullptr);
    }
    Document *jsonDocument = JsonRequestMessageParser::parseJsonMessage(jsonRequest);
    if (jsonDocument == nullptr)
    {
        handleClient_logger->warn("jsonDocument == nullptr");
        free(jsonRequest);
        free(jsonDocument);
        pthread_detach(pthread_self());
        pthread_exit(nullptr);
    }
    handleClient_logger->info("Passed parsing of the request.");

    GenericResponseMessage *message = executeRequest(currentClient, jsonDocument);
    handleClient_logger->info("Executed request...");
    sendResponseToClient(*message, currentClient->clientSocketFD);
    handleClient_logger->info("codeDescription:");
    handleClient_logger->info(message->getCodeDescription());

    free(jsonRequest);
    handleClient_logger->info("End of handleClient function.");

    pthread_detach(pthread_self());
    pthread_exit(nullptr);
}

int Server::readJsonRequestLength(const ClientInformation *currentClient)
{
    char *prefix = (char *) malloc(sizeof(char) * PREFIX_LENGTH);
    bzero(prefix, PREFIX_LENGTH);

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
        if (currentCharacter[0] == '\n')
        {
            break;
        }
        else
        {
            currentCharacter[1] = '\0';
        }
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

char *Server::readJsonRequestFromClient(const ClientInformation *currentClient, int jsonRequestLength)
{
    if (-1 == jsonRequestLength)
    {
        readJsonRequestFromClient_logger->warn("The response length is invalid.");
        return nullptr;
    }

    int totalBytesRead = 0, count = 0, totalBytesLeftToRead = jsonRequestLength + 1;
    char *jsonResponse = (char *) malloc(sizeof(char) * (jsonRequestLength + 2));
    bzero(jsonResponse, (size_t)(jsonRequestLength + 1));

    while (totalBytesLeftToRead > 0)
    {
        int bytesToReadInCurrentSession;

        (totalBytesLeftToRead < BUFF_SIZE) ?
                bytesToReadInCurrentSession = totalBytesLeftToRead :
                bytesToReadInCurrentSession = BUFF_SIZE;

        count = (int) read(currentClient->clientSocketFD, jsonResponse + totalBytesRead,
                           (size_t) bytesToReadInCurrentSession);
        switch(count)
        {
            case -1:
            {
                close(currentClient->clientSocketFD);
                readJsonRequestFromClient_logger->warn("Reading response from server failed: -1 == count");
                return nullptr;
            }
            case 0:
            {
                close(currentClient->clientSocketFD);
                readJsonRequestFromClient_logger->warn("Reading response from server failed: 0 == count");
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
        readJsonRequestFromClient_logger->warn("Reading response from server failed.");
        return nullptr;
    }

    return jsonResponse;
}

bool Server::sendResponseToClient(const GenericResponseMessage &response, int clientSocketFD)
{
    sendResponseToClient_logger->info("Inside sendResponseToClient function.");
    string jsonResponse = JsonResponseMessageGenerator::getJsonBasicResponseMessage(response);

    int length = (int) jsonResponse.length();
    char *prefixedJsonResponse = (char *) malloc(sizeof(char)*(PREFIX_LENGTH + length + 1));
    bzero(prefixedJsonResponse, sizeof(prefixedJsonResponse));
    sprintf(prefixedJsonResponse, "%d\n%s", length, jsonResponse.c_str());
    sendResponseToClient_logger->info("prefixedJsonResponse");
    sendResponseToClient_logger->info(prefixedJsonResponse);

    sendResponseToClient_logger->info("Preparing to write the response into the socket.");
    int totalBytesLeftToSend = 1 + (int) strlen(prefixedJsonResponse);
    int totalBytesSent = 0;
    int count = 0;

    while (totalBytesLeftToSend > 0)
    {
        count = (int) write(clientSocketFD, prefixedJsonResponse + totalBytesSent, BUFF_SIZE);
        switch(count)
        {
            case -1:
            {
                sendResponseToClient_logger->warn("Writing response to client failed: -1 == count");
                free(prefixedJsonResponse);
                return false;
            }
            case 0:
            {
                sendResponseToClient_logger->warn("Writing response to client failed: 0 == count");
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

    sendResponseToClient_logger->info("End of sendResponseToClient function.");
    free(prefixedJsonResponse);
    return true;
}

GenericResponseMessage *Server::executeRequest(ClientInformation *currentClient, Document *document)
{
    executeRequest_logger->info("Inside execute request");
    GenericResponseMessage *response = new GenericResponseMessage();
    response->setCode(UNKNOWN_CODE);
    response->setCodeDescription(UNKNOWN);

    if (document == nullptr)
    {
        executeRequest_logger->warn("Parsing the request failed.");
        pthread_detach(pthread_self());
        response->setCode(JSON_PARSING_FAILED_CODE);
        response->setCodeDescription(JSON_PARSING_FAILED);
        return response;
    }

    executeRequest_logger->warn("Finding out the command's name.");

    char * command = (char *) malloc(MAX_COMM_NAME_LEN * sizeof(char));
    strcpy(command, document->FindMember(COMMAND)->value.GetString());

    if (0 == strcmp(LOGIN, command))
    {
        response = executeLoginRequest(currentClient, document);
    }

    if (0 == strcmp(LOGOUT, command))
    {
        response = executeLogoutRequest(currentClient, document);
    }

    if (0 == strcmp(QUERY, command))
    {
        response = executeQuery(currentClient, document);
    }

    if (0 == strcmp(PAIR_REQUEST, command))
    {
        response = executePairRequest(currentClient, document);
    }
    return response;
}

GenericResponseMessage *Server::executeLoginRequest(ClientInformation *clientInformation, rapidjson::Document *document)
{
    GenericResponseMessage *response = new GenericResponseMessage();
    string username = document->FindMember(ARGUMENTS)->value[USERNAME].GetString();
    if (loggedUsers->find(username) == loggedUsers->cend())
    {
        // Add new user to the loggedUsers map
        User *newUserInformation = new User();
        newUserInformation->setAddress(clientInformation->address);
        loggedUsers->insert(pair<string, User>(username, *newUserInformation));
        // Send a "login approved" response back to the client
        response->setCode(LOGIN_APPROVED_CODE);
        response->setCodeDescription(LOGIN_APPROVED);
    }
    else
    {
        // Username is already registered, send a "login failed" response back to the client
        response->setCode(LOGIN_FAILED_CODE);
        response->setCodeDescription(LOGIN_FAILED);
    }
    return response;
}

GenericResponseMessage *Server::executeLogoutRequest(ClientInformation *clientInformation, rapidjson::Document *document)
{
    // Remove username from loggedUsers list
    string username = document->FindMember(ARGUMENTS)->value[USERNAME].GetString();
    loggedUsers->erase(username);

    // Remove pair if username is in a pair
    for (auto constIterator = pairs->cbegin(); constIterator != pairs->cend(); constIterator++)
    {
        if (0 == strcmp(username.c_str(),(*constIterator).first.c_str()))
        {
            pairs->erase((*constIterator).first.c_str());
            break;
        }
        if (0 == strcmp(username.c_str(),(*constIterator).second.c_str()))
        {
            pairs->erase((*constIterator).first.c_str());
            break;
        }
    }
    GenericResponseMessage *response = new GenericResponseMessage();
    response->setCode(LOGOUT_APPROVED_CODE);
    response->setCodeDescription(LOGOUT_APPROVED);
    return response;
}

GenericResponseMessage *Server::executeQuery(ClientInformation *clientInformation, rapidjson::Document *document)
{
    GenericResponseMessage *response = new GenericResponseMessage();
    string username = document->FindMember(ARGUMENTS)->value[USERNAME].GetString();

    // Client send a request to update its last connection check, server receives request and updates based on server time
    if (loggedUsers->find(username) == loggedUsers->cend())
    {
        // Username is not registered
        response->setCode(USER_NOT_LOGGED_IN_CODE);
        response->setCodeDescription(USER_NOT_LOGGED_IN);
    }
    else
    {
        // Username was found in the list
        timeval now;
        gettimeofday(&now, NULL);
        loggedUsers->at(username).updateLastCheck(now);

        query_logger->warn("update");
        query_logger->warn(username);

        response->setCode(QUERY_APPROVED_CODE);
        response->setCodeDescription(QUERY_APPROVED);
    }
    return response;
}

GenericResponseMessage *Server::executePairRequest(ClientInformation *clientInformation, Document *document)
{
    pairRequest_logger->info("Inside executePairRequest function.");
    GenericResponseMessage *response = new GenericResponseMessage();
    string sender = document->FindMember(ARGUMENTS)->value[SENDER].GetString();
    string receiver = document->FindMember(ARGUMENTS)->value[RECEIVER].GetString();

    if (usernameIsPaired(sender.c_str()))
    {
        response->setCode(ALREADY_PAIRED_CODE);
        response->setCodeDescription(ALREADY_PAIRED);
        char * expected = (char*) malloc (MAX_COMM_NAME_LEN*sizeof(char));
        for (auto constIterator = pairs->cbegin(); constIterator != pairs->cend(); constIterator++)
        {
            if(0 == strcmp((*constIterator).first.c_str(),sender.c_str())){
                strcpy(expected,(*constIterator).second.c_str());
                break;
            }
            if(0 == strcmp((*constIterator).second.c_str(),sender.c_str())){
                strcpy(expected,(*constIterator).first.c_str());
                break;
            }
        }
        response->setReceiver(expected);
        return response;
    }

    if (loggedUsers->find(sender) != loggedUsers->cend())// sender is logged in
    {
        pairRequest_logger->info("sender is logged in");
        if (loggedUsers->find(receiver) != loggedUsers->cend())//receiver is logged in
        {
            pairRequest_logger->info("receiver is logged in");
            if (0 == sender.compare(receiver))// sender is the same as receiver
            {
                pairRequest_logger->warn("sender is the same as receiver");
                response->setCode(INVITED_YOURSELF_CODE);
                response->setCodeDescription(INVITED_YOURSELF);
            }
            else // sender is different from receiver
            {
                pairRequest_logger->info("sender is different from receiver");
                if (usernameIsPaired(sender.c_str()) || usernameIsPaired(receiver.c_str()))// already paired
                {
                    pairRequest_logger->warn("sender or receiver already paired");
                    response->setCode(ALREADY_PAIRED_CODE);
                    response->setCodeDescription(ALREADY_PAIRED);
                    if (usernameIsPaired(sender.c_str()))
                    {
                        response->setCode(ALREADY_PAIRED_CODE);
                        response->setCodeDescription(ALREADY_PAIRED);
                        char * expected = (char*) malloc (MAX_COMM_NAME_LEN*sizeof(char));
                        for (auto constIterator = pairs->cbegin(); constIterator != pairs->cend(); constIterator++)
                        {
                            if(0 == strcmp((*constIterator).first.c_str(),sender.c_str())){
                                strcpy(expected,(*constIterator).second.c_str());
                                break;
                            }
                            if(0 == strcmp((*constIterator).second.c_str(),sender.c_str())){
                                strcpy(expected,(*constIterator).first.c_str());
                                break;
                            }
                        }
                        response->setReceiver(expected);
                    }
                }
                else
                {
                    pairRequest_logger->info("sender and receiver have not been paired yet");
                    if (!usernameIsPaired(sender.c_str()) && !usernameIsPaired(receiver.c_str()))//not already paired
                    {
                        pairRequest_logger->info("receiver accepted pair invitation");
                        pairs->insert(pair<string, string>(sender, receiver));
                        response->setCode(PAIR_ADDED_CODE);
                        response->setCodeDescription(PAIR_ADDED);
                    }
                }
            }
        }
        else//receiver not logged in
        {
            pairRequest_logger->info("receiver not logged in");
            response->setCode(USER_NOT_LOGGED_IN_CODE);
            response->setCodeDescription(USER_NOT_LOGGED_IN);
        }
    }
    else// sender not logged in
    {
        pairRequest_logger->info("sender not logged in");
        response->setCode(USER_NOT_LOGGED_IN_CODE);
        response->setCodeDescription(USER_NOT_LOGGED_IN);
    }

    return response;
}

void Server::disconnectInactiveClients()
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
            timeval now;
            gettimeofday(&now, NULL);
            if (now.tv_sec - usersIT->second.getLastCheck().tv_sec > 10)
            {
                // Delete username from logged users
                loggedUsers->erase(usersIT);
                //or usersIT->first
                handleDisconnecting_logger->info("Server erased a client.");

                // eliminate pair containing this username
                for (auto pairsIt = pairs->cbegin(); pairsIt != pairs->cend(); pairsIt++)
                {
                    if (0 == pairsIt->first.compare(usersIT->first.c_str()))
                    {
                        pairs->erase(pairsIt);
                        break;
                    }
                    if (0 == pairsIt->second.compare(usersIT->first.c_str()))
                    {
                        pairs->erase(pairsIt);
                        break;
                    }
                }
            }
        }

        handleDisconnecting_logger->info("Server's disconnecting service will sleep for 10 seconds.");
        sleep(10);
    }
}

bool Server::stringContainsOnlyDigits(char *string)
{
    char *end;
    long converted = strtol(string, &end, 10);
    if (*end)
    {
        // Conversion failed because the input wasn't a number.
        return false;
    }
    return true;
}

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

bool Server::usernameIsPaired(const char* username)
{
    for (auto constIterator = pairs->cbegin(); constIterator != pairs->cend(); constIterator++)
    {
        if(0 == strcmp((*constIterator).first.c_str(),username)){
            return true;
        }
        if(0 == strcmp((*constIterator).second.c_str(),username)){
            return true;
        }
    }
    return false;
}

