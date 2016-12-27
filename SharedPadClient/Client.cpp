#include "Client.h"

const char *Client::ip = "127.0.0.1";
const in_port_t Client::port = 2024;
sockaddr_in Client::serverConfiguration;

auto sendRequestToServer_logger = spd::stdout_color_mt("sendRequestToServer_logger");
auto establishConnection_logger = spd::stdout_color_mt("establishConnection_logger");
auto readJsonResponseLengthFromServer_logger = spd::stdout_color_mt("readJsonResponseLengthFromServer_logger");
auto readJsonResponseFromServer_logger = spd::stdout_color_mt("readJsonResponseFromServer_logger");

Client::Client()
{
    // Preparing the data structure user by the server
    bzero(&serverConfiguration, sizeof(sockaddr_in));
    serverConfiguration.sin_family = AF_INET;
    inet_aton(ip, &serverConfiguration.sin_addr);
    serverConfiguration.sin_port = htons(port);

}

int Client::establishConnection()
{
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == socketFD)
    {
        establishConnection_logger->warn("Socket creation failed.");
        return -1;
    }

    if (connect(socketFD, (sockaddr *) &serverConfiguration, sizeof (sockaddr)) == -1)
    {
        establishConnection_logger->warn("Server connection failed.");
        return -1;
    }

    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    if (-1 == setsockopt(socketFD, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout)))
    {
        establishConnection_logger->warn("Setting socket option (SO_RCVTIMEO) failed.");
        return -1;
    }

    if (-1 == setsockopt(socketFD, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(timeout)))
    {
        establishConnection_logger->warn("Setting socket option (SO_SNDTIMEO) failed.");
        return -1;
    }

    return socketFD;
}

GenericResponseMessage* Client::login(std::string username){

    GenericRequestMessage loginRequest;
    loginRequest.setCommand(LOGIN);
    loginRequest.setUsername(username);

    std::string jsonLoginRequest = JsonRequestMessageGenerator::getJsonLoginRequestMessage(loginRequest);
    GenericResponseMessage* responseFromServer = Client::sendRequestToServer(jsonLoginRequest);
    return responseFromServer;
}

int Client::readJsonResponseLengthFromServer(int socketFD)
{
    char currentCharacter[2]; // The string must end in '\0'
    int totalBytesRead = 0;
    int count = 0;
    char *prefix = (char *) malloc(sizeof(char) * PREFIX_LENGTH);
    bzero(prefix, PREFIX_LENGTH);

    // Read character after character until '\n' is encountered in order to obtain message length
    while (PREFIX_LENGTH > totalBytesRead)
    {
        count = (int) read(socketFD, &currentCharacter, 1);
        if (-1 == count)
        {
            close(socketFD);
            readJsonResponseLengthFromServer_logger->warn("Reading the response failed.");
            return -1;
        }
        if (currentCharacter[0] == '\n'){break;}
        else {currentCharacter[1] ='\0';}
        strcat(prefix, currentCharacter);
        totalBytesRead += count;
    }

    if (!stringContainsOnlyDigits(prefix))
    {
        close(socketFD);
        readJsonResponseLengthFromServer_logger->warn("The prefix of the response contained invalid characters.");
        return -1;
    }

    return atoi(prefix);
}

char *Client::readJsonResponseFromServer(int socketFD, int jsonResponseLength)
{
    if (-1 == jsonResponseLength){
        readJsonResponseFromServer_logger->warn("The response length is invalid.");
        return nullptr;
    }

    int totalBytesRead = 0;
    int count = 0;
    int totalBytesLeftToRead = jsonResponseLength + 1;
    char *jsonResponse = (char *) malloc(sizeof(char) * (jsonResponseLength + 2));
    bzero(jsonResponse, jsonResponseLength + 1);

    while (totalBytesLeftToRead > 0)
    {
        int bytesToReadInCurrentSession;

        (totalBytesLeftToRead < BUFF_SIZE) ?
                bytesToReadInCurrentSession = totalBytesLeftToRead :
                bytesToReadInCurrentSession = BUFF_SIZE;

        count = (int) read(socketFD, jsonResponse + totalBytesRead,
                           (size_t) bytesToReadInCurrentSession);
        if (-1 == count)
        {
            close(socketFD);
            readJsonResponseFromServer_logger->warn("Reading response from server failed.");
            return nullptr;
        }
        if (0 == count){break;}
        totalBytesRead += count;
        totalBytesLeftToRead -= count;
    }

    if (jsonResponse == nullptr)
    {
        readJsonResponseFromServer_logger->warn("Reading response from server failed.");
        return nullptr;
    }

    return jsonResponse;
}

GenericResponseMessage* Client::sendRequestToServer(std::string jsonRequest){

    GenericResponseMessage* response = new GenericResponseMessage();

    sendRequestToServer_logger->info("Inside sendRequestToServer function.");

    int socketFD = Client::establishConnection();

    if (-1 == socketFD){
        close(socketFD);
        sendRequestToServer_logger->warn("Establishing connection with the server failed...");
        response->setCode(CONNECTION_FAILED_CODE);
        response->setCodeDescription(CONNECTION_FAILED);
        return response;
    }
    sendRequestToServer_logger->info("jsonRequest");
    sendRequestToServer_logger->info(jsonRequest);

    // Must add a prefix with the length of the request
    sendRequestToServer_logger->info("prefixedJsonRequest");
    int length = (int) jsonRequest.length();
    char *prefixedJsonRequest = (char *) malloc((size_t) (PREFIX_LENGTH + length + 1));
    bzero(prefixedJsonRequest, PREFIX_LENGTH + length + 1);
    sprintf(prefixedJsonRequest, "%d\n%s", length, jsonRequest.c_str());
    sendRequestToServer_logger->info(prefixedJsonRequest);

    // Write request into socket
    sendRequestToServer_logger->info("Preparing to write the request into the socket.");
    int totalBytesLeftToSend = 1 + (int) strlen(prefixedJsonRequest);
    int totalBytesSent = 0;
    int count = 0;

    while (totalBytesLeftToSend > 0)
    {
        count = (int) write(socketFD, prefixedJsonRequest + totalBytesSent, BUFF_SIZE);
        if (-1 == count)
        {
            close(socketFD);
            free(prefixedJsonRequest);
            sendRequestToServer_logger->warn("Writing request to server failed: -1 == count");
            response->setCode(WRITE_FAILED_CODE);
            response->setCodeDescription(WRITE_FAILED);
            return response;
        } else if (0 == count)//connection closed meanwhile
        {
            free(prefixedJsonRequest);
            sendRequestToServer_logger->warn("Writing request to server failed: 0 == count");
            response->setCode(WRITE_FAILED_CODE);
            response->setCodeDescription(WRITE_FAILED);
            return response;
        }
        totalBytesSent += count;
        totalBytesLeftToSend -= count;
    }

    sendRequestToServer_logger->info("Finished writing the request into the socket.");
    sendRequestToServer_logger->info("Reading the response from the server.");

    // Reading server's response to the client's request
    sendRequestToServer_logger->info("jsonResponseLength:");
    length = Client::readJsonResponseLengthFromServer(socketFD);
    sendRequestToServer_logger->info(length);
    sendRequestToServer_logger->info("responseFromServer:");
    char * responseFromServer = Client::readJsonResponseFromServer(socketFD,length);
    sendRequestToServer_logger->info(responseFromServer);

    Document *jsonDocument = JsonResponseMessageParser::parseJsonMessage(responseFromServer);
    free(responseFromServer);

    if (jsonDocument == nullptr)
    {
        response->setCode(JSON_PARSING_FAILED_CODE);
        response->setCodeDescription(JSON_PARSING_FAILED);
        sendRequestToServer_logger->warn("jsonDocument is nullptr");
        return response;
    }

    // If it reaches this point, the response from the server will contain the actual result of the request made by the client
    Document newDocument;
    newDocument.CopyFrom(*jsonDocument, newDocument.GetAllocator());
    response->setCode(newDocument[CODE].GetInt());
    response->setCodeDescription(newDocument[CODE_DESCRIPTION].GetString());
    close(socketFD);

    sendRequestToServer_logger->info("End of sendRequestToServer function...");
    return response;
}

bool Client::stringContainsOnlyDigits(char *string)
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
