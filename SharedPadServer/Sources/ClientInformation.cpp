#include <string.h>
#include "../Headers/ClientInformation.h"

ClientInformation::ClientInformation()
{
    bzero(&address, sizeof(sockaddr_in));
    addressLength = sizeof(sockaddr);
}
