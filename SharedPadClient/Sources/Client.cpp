#include "Client.h"

sockaddr_in Client::serverConfiguration;
const char *Client::ip = "127.0.0.1";
const in_port_t Client::port = 2024;

auto sendRequest_logger = spd::stdout_color_mt("sendRequest_logger");
auto readJsonResponseLength_logger = spd::stdout_color_mt("readJsonResponseLength_logger");
auto readJsonResponse_logger = spd::stdout_color_mt("readJsonResponse_logger");

Client::Client()
{
    bzero(&serverConfiguration, sizeof(sockaddr_in));
    serverConfiguration.sin_family = AF_INET;
    inet_aton(ip, &serverConfiguration.sin_addr);
    serverConfiguration.sin_port = htons(port);
}

GenericResponse* Client::login(string username)
{
    GenericRequest *loginRequest = SpecializedRequest::getLoginRequest(username);
    string jsonLoginRequest = JsonRequestGenerator::getJsonLogRequest(*loginRequest);
    GenericResponse* responseFromServer = Client::sendRequest(jsonLoginRequest);
    return responseFromServer;
}

GenericResponse* Client::pair(string sender, string receiver){

    GenericRequest *pairRequest = SpecializedRequest::getPairRequest(sender, receiver);
    string jsonPairRequest = JsonRequestGenerator::getJsonPairRequest(*pairRequest);
    GenericResponse* responseFromServer = Client::sendRequest(jsonPairRequest);
    return responseFromServer;
}

GenericResponse * Client::sendNews(string username, string content)
{
    GenericRequest *sendNewsRequest = SpecializedRequest::getSendNewsRequest(content, username);
    string jsonSyncRequest = JsonRequestGenerator::getJsonSyncRequest(*sendNewsRequest);
    GenericResponse* responseFromServer = Client::sendRequest(jsonSyncRequest);
    return responseFromServer;
}

/**
 * The client must first connect to the server, then add a prefix to the message, send it and wait for a response.
 * This function returns the result of the request, meaning the response from the server.
*/
GenericResponse *Client::sendRequest(string jsonRequest){

    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == socketFD)
    {
        close(socketFD);
        sendRequest_logger->warn("Establishing connection with the server failed.");
        return Client::getConnectionFailedResponse();
    }
    if (connect(socketFD, (sockaddr *) &serverConfiguration, sizeof (sockaddr)) == -1)
    {
        close(socketFD);
        sendRequest_logger->warn("Establishing connection with the server failed.");
        return Client::getConnectionFailedResponse();
    }

    int length = (int) jsonRequest.length();
    char *prefixedJsonRequest = getPrefixedJsonRequest(jsonRequest, length);

    sendRequest_logger->info("Preparing to write the request into the socket.");
    int totalBytesLeftToSend = 1 + (int) strlen(prefixedJsonRequest);
    int totalBytesSent = 0, count = 0;

    while (totalBytesLeftToSend > 0)
    {
        count = (int) write(socketFD, prefixedJsonRequest + totalBytesSent, BUFF_SIZE);
        switch(count)
        {
        case -1:
        {
            close(socketFD);
            free(prefixedJsonRequest);
            sendRequest_logger->warn("Writing request to server failed: -1 == count");
            return Client::getWriteFailedResponse();
        }
        case 0:
        {
            close(socketFD);
            free(prefixedJsonRequest);
            sendRequest_logger->warn("Writing request to server failed: 0 == count");
            return Client::getWriteFailedResponse();
        }
        default:
        {
            totalBytesSent += count;
            totalBytesLeftToSend -= count;
        }
        }
    }

    length = Client::readJsonResponseLength(socketFD);
    char * responseFromServer = Client::readJsonResponse(socketFD,length);
    sendRequest_logger->info("Reading the response from the server. The response is:");
    sendRequest_logger->info(responseFromServer);

    Document *jsonDocument = JsonResponseParser::parseJson(responseFromServer);
    free(responseFromServer);
    if (jsonDocument == nullptr)
    {
        sendRequest_logger->warn("jsonDocument is nullptr");
        return Client::getJsonParsingFailedResponse();
    }

    // If it reaches this point, the response from the server will contain the actual result of the request made by the client
    Document newDocument;
    newDocument.CopyFrom(*jsonDocument, newDocument.GetAllocator());
    GenericResponse * response = new GenericResponse();
    response->setCode(newDocument[CODE].GetInt());
    response->setCodeDescription(newDocument[CODE_DESCRIPTION].GetString());

    close(socketFD);
    free(jsonDocument);

    return response;
}

char * Client::getPrefixedJsonRequest(string jsonRequest, int length)
{
    char *prefixedJsonRequest = (char *) malloc((size_t) (PREFIX_LENGTH + length + 1));
    memset(prefixedJsonRequest, 0, sizeof(char) *(PREFIX_LENGTH + length + 1));
    sprintf(prefixedJsonRequest, "%d\n%s", length, jsonRequest.c_str());
    sendRequest_logger->info(prefixedJsonRequest);
    return prefixedJsonRequest;
}


int Client::readJsonResponseLength(int socketFD)
{
    char *prefix = (char *) malloc(sizeof(char) * (PREFIX_LENGTH));
    memset(prefix, 0, sizeof(char) * PREFIX_LENGTH);

    char currentCharacter[2];
    int totalBytesRead = 0, count = 0;

    while (PREFIX_LENGTH > totalBytesRead)
    {
        count = (int) read(socketFD, &currentCharacter, 1);
        if (-1 == count)
        {
            close(socketFD);
            readJsonResponseLength_logger->warn("Reading the response failed.");
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
        readJsonResponseLength_logger->warn("The prefix of the response contained invalid characters.");
        return -1;
    }

    return atoi(prefix);
}

char *Client::readJsonResponse(int socketFD, int jsonResponseLength)
{
    if (-1 == jsonResponseLength)
    {
        readJsonResponse_logger->warn("The response length is invalid.");
        return nullptr;
    }

    int totalBytesRead = 0, count = 0, totalBytesLeftToRead = jsonResponseLength + 1;
    char *jsonResponse = (char *) malloc(sizeof(char) * (jsonResponseLength + 2));

    while (totalBytesLeftToRead > 0)
    {
        int bytesToReadInCurrentSession = Client::getBytesToReadInCurrentSession(totalBytesLeftToRead);

        count = (int) read(socketFD, jsonResponse + totalBytesRead,
                           (size_t) bytesToReadInCurrentSession);
        switch(count)
        {
        case -1:
        {
            close(socketFD);
            readJsonResponse_logger->warn("Reading response failed: -1 == count");
            return nullptr;
        }
        case 0:
        {
            close(socketFD);
            readJsonResponse_logger->warn("Reading response failed: 0 == count");
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
        readJsonResponse_logger->warn("Reading response failed.");
        return nullptr;
    }

    return jsonResponse;
}

int Client::getBytesToReadInCurrentSession(int total)
{
    int number;
    total < BUFF_SIZE ? number = total : number = BUFF_SIZE;
    return number;
}

GenericResponse *Client::getWriteFailedResponse()
{
    GenericResponse* response = new GenericResponse();
    response->setCode(WRITE_FAILED_CODE);
    response->setCodeDescription(WRITE_FAILED);
    return response;
}

GenericResponse* Client::getConnectionFailedResponse()
{
    GenericResponse* response = new GenericResponse();
    response->setCode(CONNECTION_FAILED_CODE);
    response->setCodeDescription(CONNECTION_FAILED);
    return response;
}

GenericResponse* Client::getJsonParsingFailedResponse()
{
    GenericResponse* response = new GenericResponse();
    response->setCode(JSON_PARSING_FAILED_CODE);
    response->setCodeDescription(JSON_PARSING_FAILED);
    return response;
}

bool Client::stringContainsOnlyDigits(char *string)
{
    char *end;
    strtol(string, &end, 10);
    if (*end)
    {
        // Conversion failed because the input wasn't a number.
        return false;
    }
    return true;
}
