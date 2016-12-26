#include <arpa/inet.h>
#include <cstdio>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include "Client.h"
#include "ErrorHandler.h"
#include "NamespaceSPP.h"
#include "JsonResponseMessageParser.h"
#include "JsonRequestMessageGenerator.h"
#include "GenericRequestMessage.h"
#include "GenericResponseMessage.h"
#include "rapidjson/document.h"

const char *Client::ip = "127.0.0.1";
const in_port_t Client::port = 2024;
sockaddr_in Client::serverConfiguration;

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
        ErrorHandler::handleErrorWithoutExit("Socket creation failed.\n");
        return -1;
    }

    if (connect(socketFD, (sockaddr *) &serverConfiguration, sizeof (sockaddr)) == -1)
    {
        ErrorHandler::handleErrorWithoutExit("Server connection failed.\n");
        return -1;
    }

    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    if (-1 == setsockopt(socketFD, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout)))
    {
        ErrorHandler::handleErrorWithoutExit("Setting socket option (SO_RCVTIMEO) failed.\n");
        return -1;
    }

    if (-1 == setsockopt(socketFD, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(timeout)))
    {
        ErrorHandler::handleErrorWithoutExit("Setting socket option (SO_SNDTIMEO) failed.\n");
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
    // Read character after character until '\n' is encountered, to obtain message length
    int totalBytesRead = 0;
    int count = 0;
    char currentCharacter[2];

    char *prefix = (char *) malloc(sizeof(char) * PREFIX_LENGTH);
    bzero(prefix, PREFIX_LENGTH);

    while (totalBytesRead < PREFIX_LENGTH)
    {
        count = (int) read(socketFD, &currentCharacter, 1);
        if (-1 == count)
        {
            close(socketFD);
            ErrorHandler::handleErrorWithoutExit("Reading the response failed.\n");
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
        close(socketFD);
        ErrorHandler::handleErrorWithoutExit("The prefix of the response contained invalid characters.\n");
        return -1;
    }

    return atoi(prefix);
}

char *Client::readJsonResponseFromServer(int socketFD, int jsonResponseLength)
{
    if (-1 == jsonResponseLength){
        ErrorHandler::handleErrorWithoutExit("The response length is invalid.\n");
        return nullptr;
    }

    char *jsonResponse = (char *) malloc(sizeof(char) * (jsonResponseLength + 1));
    bzero(jsonResponse, jsonResponseLength + 1);

    int totalBytesRead = 0;
    int count = 0;
    int totalBytesLeftToRead = jsonResponseLength + 1;

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
            ErrorHandler::handleErrorWithoutExit("Reading response from server failed.\n");
            return nullptr;
        }
        if (count == 0)//socket closed meanwhile
        {
            break;
        }
        totalBytesRead += count;
        totalBytesLeftToRead -= count;
    }

    if (jsonResponse == nullptr)
    {
        ErrorHandler::handleErrorWithoutExit("Reading response from server failed.\n");
        return nullptr;
    }

    return jsonResponse;
}

GenericResponseMessage* Client::sendRequestToServer(std::string jsonRequest){

    GenericResponseMessage* response = new GenericResponseMessage();
    namespace spd = spdlog;
    auto logger = spd::stdout_color_mt("logger");
    logger->info("sendRequestToServer()");

    // Establish communication with the server
    int socketFD = Client::establishConnection();

    if (-1 == socketFD){
        logger->warn("Establishing connection failed.");
        response->setCode(CONNECTION_FAILED_CODE);
        response->setCodeDescription(CONNECTION_FAILED);
        ErrorHandler::handleErrorWithoutExit("Establishing connection failed.\n");
        close(socketFD);
        return response;
    }

    // Must add a prefix with the length of the request
    int length = (int) jsonRequest.length();
    char *prefixedJsonRequest = (char *) malloc((size_t) (PREFIX_LENGTH + length + 1));
    bzero(prefixedJsonRequest, PREFIX_LENGTH + length + 1);
    sprintf(prefixedJsonRequest, "%d\n%s", length, jsonRequest.c_str());

    // Write request into socket
    int totalBytesLeftToSend = 1 + (int) strlen(prefixedJsonRequest);
    int totalBytesSent = 0;
    int count = 0;

    while (totalBytesLeftToSend > 0)
    {
        count = (int) write(socketFD, prefixedJsonRequest + totalBytesSent, BUFF_SIZE);
        if (-1 == count)
        {
            close(socketFD);
            response->setCode(WRITE_FAILED_CODE);
            response->setCodeDescription(WRITE_FAILED);
            ErrorHandler::handleErrorWithoutExit("Writing request to server failed.\n");
            free(prefixedJsonRequest);
            return response;
        } else if (0 == count)//connection closed meanwhile
        {
            logger->info("count == 0");
            free(prefixedJsonRequest);
        }
        totalBytesSent += count;
        totalBytesLeftToSend -= count;
    }

    // Reading server's response to the client's request
    length = Client::readJsonResponseLengthFromServer(socketFD);
    char * responseFromServer = Client::readJsonResponseFromServer(socketFD,length);

    rapidjson::Document *jsonDocument = JsonResponseMessageParser::parseJsonMessage(responseFromServer);
    free(responseFromServer);

    if (jsonDocument == nullptr)
    {
        logger->warn("jsonDocument is nullptr");
        response->setCode(JSON_PARSING_FAILED_CODE);
        response->setCodeDescription(JSON_PARSING_FAILED);
        ErrorHandler::handleErrorWithoutExit("Json document is nullptr. Parsing the response failed.\n");
        return response;
    }

    // If it reaches this point, the response from the server will contain the actual result of the request made by the client
    rapidjson::Document newDocument;
    // The value from source(document) is moved, not copied, to destination (newDocument)
    newDocument.CopyFrom(*jsonDocument, newDocument.GetAllocator());
    response->setCode(newDocument[CODE].GetInt());
    response->setCodeDescription(newDocument[CODE_DESCRIPTION].GetString());
    logger->info("End of function.");
    close(socketFD);
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
