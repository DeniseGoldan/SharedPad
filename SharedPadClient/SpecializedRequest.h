#ifndef CLIENT_SPECIALIZEDREQUEST_H
#define CLIENT_SPECIALIZEDREQUEST_H

#include <string.h>

#include "StatusCodesAndDescriptions.h"
#include "GenericRequest.h"

using namespace std;

class SpecializedRequest
{
public:
    SpecializedRequest();

    static GenericRequest *getLoginRequest(string username);

    static GenericRequest *getPairRequest(string sender, string receiver);

    static GenericRequest *getSendNewsRequest(string content, string username);

};

#endif // CLIENT_SPECIALIZEDREQUEST_H
