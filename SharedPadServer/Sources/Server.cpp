#include "../Headers/Server.h"

const char *Server::ip = "127.0.0.1";
const in_port_t Server::port = 2024;
sockaddr_in Server::serverConfiguration;

map<string, User> *Server::loggedUsers;

auto startListeningSession_logger = spd::stdout_color_mt("startListeningSession_logger");
auto handleClient_logger = spd::stdout_color_mt("handleClient_logger");
auto readJsonRequestLength_logger = spd::stdout_color_mt("readJsonRequestLength_logger");
auto readJsonRequestFromClient_logger = spd::stdout_color_mt("readJsonRequestFromClient_logger");
auto sendResponseToClient_logger = spd::stdout_color_mt("sendResponseToClient_logger");
auto executeRequest_logger = spd::stdout_color_mt("executeRequest_logger");
auto handleDisconnecting_logger = spd::stdout_color_mt("handleDisconnecting_logger");


Server::Server()
{
    // Preparing the data structure user by the server
    bzero(&serverConfiguration, sizeof(sockaddr_in));
    serverConfiguration.sin_family = AF_INET;
    inet_aton(ip, &serverConfiguration.sin_addr);
    serverConfiguration.sin_port = htons(port);

    // Initializing the map containing the clients
    loggedUsers = new map<string, User>();

    //handleDisconnecting(nullptr);
}

void Server::startListeningSession()
{
    // Creating a socket
    int serverSocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == serverSocketFD)
    {
        ErrorHandler::exitFailure("Socket creation failed.\n");
    }

    // Setting the REUSEADDR option
    int enable = 1;
    if (-1 == setsockopt(serverSocketFD, SOL_SOCKET, SO_REUSEADDR, &enable, (socklen_t) sizeof(enable)))
    {
        ErrorHandler::exitFailure("Setting socket option (SO_REUSEADDR) failed.\n");
    }

    // Attaching the socket to the server configuration initialized in the constructor
    if (-1 == bind(serverSocketFD, (const sockaddr *) &serverConfiguration, sizeof(sockaddr)))
    {
        ErrorHandler::exitFailure("Bind failed.\n");
    }

    // Setting socket backlog to "SOcket MAX CONNections"
    if (-1 == listen(serverSocketFD, SOMAXCONN))
    {
        ErrorHandler::exitFailure("Listening failed.\n");
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
            startListeningSession_logger->warn("ClientInformation acceptance failed.\n");
            continue;
        }

        //Server::enableKeepAlive(currentClient->clientSocketFD);

        // Concurrency of the server comes from the usage of threads
        pthread_t threadId;
        if (0 != pthread_create(&threadId, nullptr, Server::handleClient, (void *) currentClient))
        {
            // Send a RESPONSE to the client about the error
            GenericResponseMessage response;
            response.setCode(THREAD_CREATION_FAILED_CODE);
            response.setCodeDescription("Thread creation failed.\n");
            sendResponseToClient(response, currentClient->clientSocketFD);
            // EXIT_FAILURE
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

    // Read the request from the client's socket
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

    // Execute request and send a response back to the client
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
    char currentCharacter[2]; // The string must end in '\0'
    int totalBytesRead = 0;
    int count = 0;
    char *prefix = (char *) malloc(sizeof(char) * PREFIX_LENGTH);
    bzero(prefix, PREFIX_LENGTH);

    // Read character after character until '\n' is encountered in order to obtain message length
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
        { break; }
        else
        { currentCharacter[1] = '\0'; }
        strcat(prefix, currentCharacter);
        totalBytesRead += count;
    }

    if (!stringContainsOnlyDigits(prefix))
    {
        pthread_detach(pthread_self());
        readJsonRequestLength_logger->warn("The prefix of the request contained invalid characters.\n");
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

    int totalBytesRead = 0;
    int count = 0;
    int totalBytesLeftToRead = jsonRequestLength + 1;
    char *jsonResponse = (char *) malloc(sizeof(char) * (jsonRequestLength + 2));
    bzero(jsonResponse, jsonRequestLength + 1);

    while (totalBytesLeftToRead > 0)
    {
        int bytesToReadInCurrentSession;

        (totalBytesLeftToRead < BUFF_SIZE) ?
                bytesToReadInCurrentSession = totalBytesLeftToRead :
                bytesToReadInCurrentSession = BUFF_SIZE;

        count = (int) read(currentClient->clientSocketFD, jsonResponse + totalBytesRead,
                           (size_t) bytesToReadInCurrentSession);
        if (-1 == count)
        {
            close(currentClient->clientSocketFD);
            readJsonRequestFromClient_logger->warn("Reading response from server failed.");
            return nullptr;
        }
        if (0 == count)
        { break; }
        totalBytesRead += count;
        totalBytesLeftToRead -= count;
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
    // Turn the generic response into a string
    string jsonResponse = JsonResponseMessageGenerator::getJsonBasicResponseMessage(response);

    // Must add a prefix with the length of the response and a delimiter ('\n')
    int length = (int) jsonResponse.length();
    char *prefixedJsonResponse = (char *) malloc((size_t) (PREFIX_LENGTH + length + 1));
    bzero(prefixedJsonResponse, sizeof(prefixedJsonResponse));
    sprintf(prefixedJsonResponse, "%d\n%s", length, jsonResponse.c_str());
    sendResponseToClient_logger->info("prefixedJsonResponse");
    sendResponseToClient_logger->info(prefixedJsonResponse);

    // Write response into socket
    sendResponseToClient_logger->info("Preparing to write the response into the socket.");
    int totalBytesLeftToSend = 1 + (int) strlen(prefixedJsonResponse);
    int totalBytesSent = 0;
    int count = 0;

    while (totalBytesLeftToSend > 0)
    {
        count = (int) write(clientSocketFD, prefixedJsonResponse + totalBytesSent, BUFF_SIZE);
        if (-1 == count)
        {
            sendResponseToClient_logger->warn("Writing response to client failed: -1 == count");
            free(prefixedJsonResponse);
            return false;
        } else if (0 == count)
        {
            sendResponseToClient_logger->warn("Writing response to client failed: 0 == count");
            free(prefixedJsonResponse);
            return false;
        }
        totalBytesSent += count;
        totalBytesLeftToSend -= count;
    }

    sendResponseToClient_logger->info("End of sendResponseToClient function.");
    free(prefixedJsonResponse);
    return true;
    //http://codereview.stackexchange.com/questions/27213/sending-large-data-package-with-tcp-winsock
    //http://stackoverflow.com/questions/1577825/unix-sockets-how-to-send-really-big-data-with-one-send-call
}

GenericResponseMessage *Server::executeRequest(ClientInformation *currentClient, Document *document)
{
    //This basic response will change if the request will be properly executed
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

    //string currentCommand = document->FindMember(COMMAND)->value.GetString();

    // LOGIN
    if (0 == strcmp(LOGIN, document->FindMember(COMMAND)->value.GetString()))
    {
        response = executeLoginRequest(currentClient, document);
    }

    // LOGOUT
    if (0 == strcmp(LOGOUT, document->FindMember(COMMAND)->value.GetString()))
    {
        response = executeLogoutRequest(currentClient, document);
    }

    // UPDATE_CHECK
    if (0 == strcmp(UPDATE_CONN_TEST, document->FindMember(COMMAND)->value.GetString()))
    {
        response = executeUpdateConnectionLivenessTestRequest(currentClient, document);
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
        User *newUser = new User();
        newUser->setAddress(clientInformation->address);
        newUser->setGroup(-1);
        loggedUsers->insert(pair<string, User>(username, *newUser));
        // Send a "login approved" response back to the client
        response->setCode(LOGIN_APPROVED_CODE);
        response->setCodeDescription(LOGIN_APPROVED);
    } else
    {
        // Username is already registered, send a "login failed" response back to the client
        response->setCode(LOGIN_FAILED_CODE);
        response->setCodeDescription(LOGIN_FAILED);
    }

    return response;
}

GenericResponseMessage *
Server::executeLogoutRequest(ClientInformation *clientInformation, rapidjson::Document *document)
{
    // Remove username from loggedUsers list
    string username = document->FindMember(ARGUMENTS)->value[USERNAME].GetString();
    loggedUsers->erase(username);

    GenericResponseMessage *response = new GenericResponseMessage();
    response->setCode(LOGOUT_APPROVED_CODE);
    response->setCodeDescription(LOGOUT_APPROVED);
    return response;
}

GenericResponseMessage *Server::executeUpdateConnectionLivenessTestRequest(ClientInformation *clientInformation,
                                                                           rapidjson::Document *document)
{
    // TODO: Make the client send its own time?
    GenericResponseMessage *response = new GenericResponseMessage();
    string username = document->FindMember(ARGUMENTS)->value[USERNAME].GetString();

    // Client send a request to update its last connection check, server receives request and updates based on server time
    if (loggedUsers->find(username) == loggedUsers->cend())
    {
        // Username is not registered
        response->setCode(USER_NOT_LOGGED_IN_CODE);
        response->setCodeDescription(USER_NOT_LOGGED_IN);
    } else
    {
        // Username was found in the list
        timeval now;
        gettimeofday(&now, NULL);
        loggedUsers->at(username).updateCheck(now);

        response->setCode(UPDATE_CONN_TEST_APPROVED_CODE);
        response->setCodeDescription(UPDATE_CONN_TEST_APPROVED);
    }

    return response;
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

void Server::enableKeepAlive(int socketDescriptor)
{
    int yes = 1;
    if (-1 == setsockopt(socketDescriptor, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes)))
    {
        ErrorHandler::exitFailure("Setting socket option (SO_KEEPALIVE) failed.\n");
    }

    int idle = 1;
    if (-1 == setsockopt(socketDescriptor, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle)))
    {
        ErrorHandler::exitFailure("Setting socket option (SO_KEEPIDLE) failed.\n");
    }

    int interval = 1;
    if (-1 == setsockopt(socketDescriptor, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(interval)))
    {
        ErrorHandler::exitFailure("Setting socket option (TCP_KEEPINTVL) failed.\n");
    }

    int maxNumberOfPackets = 10;
    if (-1 == setsockopt(socketDescriptor, IPPROTO_TCP, TCP_KEEPCNT, &maxNumberOfPackets, sizeof(maxNumberOfPackets)))
    {
        ErrorHandler::exitFailure("Setting socket option (TCP_KEEPCNT) failed.\n");
    }
}

void Server::disconnectInactiveClients()
{
    pthread_t threadId;
    if (0 != pthread_create(&threadId, nullptr, Server::handleDisconnecting, nullptr))
    {
        //EXIT_FAILURE
        ErrorHandler::exitFailure("Thread creation failed.\n");
    }
}

void *Server::handleDisconnecting(void *)
{
    //TODO: mutex?
    handleDisconnecting_logger->info("Inside function handleDisconnecting.");
    while (true)
    {
        for (auto constIterator = loggedUsers->cbegin(); constIterator != loggedUsers->cend(); constIterator++)
        {
            timeval now;
            gettimeofday(&now, NULL);
            if (now.tv_sec - constIterator->second.getLastCheck().tv_sec > 10)
            {
                // Delete username from logged users
                loggedUsers->erase(constIterator->first);
                handleDisconnecting_logger->info("Server erased a client.");
            }
        }
        handleDisconnecting_logger->info("Server will sleep for 10 seconds.");
        sleep(10);
    }
    handleDisconnecting_logger->info("Server finished. No clients left.");
    pthread_detach(pthread_self());
    pthread_exit(nullptr);
}
