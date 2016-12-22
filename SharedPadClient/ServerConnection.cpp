#include "ServerConnection.h"
#include "ErrorHandler.h"
#include <arpa/inet.h>
#include <cstdio>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>

void ServerConnection::establishConnection()
{
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == socketFD)
    {
        ErrorHandler::handleErrorAndExitFailure("Socket creation failed.\n");
    }

    bzero(&configuration, sizeof(sockaddr_in));

    configuration.sin_family = AF_INET;
    inet_aton(ip, &configuration.sin_addr);
    configuration.sin_port = htons(port);

    if (connect(socketFD, (sockaddr *) &configuration, sizeof (sockaddr)) == -1)
    {
        ErrorHandler::handleErrorAndExitFailure("Server connection failed.\n");
    }
}
