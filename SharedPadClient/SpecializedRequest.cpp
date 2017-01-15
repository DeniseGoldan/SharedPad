#include "SpecializedRequest.h"

SpecializedRequest::SpecializedRequest(){}

GenericRequest *SpecializedRequest::getLoginRequest(string username)
{
    GenericRequest *loginRequest = new GenericRequest();
    loginRequest->setCommand(LOGIN);
    loginRequest->setUsername(username);

    return loginRequest;
}

GenericRequest *SpecializedRequest::getPairRequest(string sender, string receiver)
{
    GenericRequest *pairRequest = new GenericRequest();
    pairRequest->setCommand(PAIR);
    pairRequest->setSender(sender);
    pairRequest->setReceiver(receiver);
    return pairRequest;
}

GenericRequest *SpecializedRequest::getSendNewsRequest(string content, string username)
{
    GenericRequest *sendNewsRequest = new GenericRequest();
    sendNewsRequest->setCommand(SEND_NEWS);
    sendNewsRequest->setUsername(username);
    sendNewsRequest->setContent(content);
    return sendNewsRequest;
}
