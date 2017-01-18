#include "SpecializedRequest.h"

SpecializedRequest::SpecializedRequest(){}

GenericRequest *SpecializedRequest::getLoginRequest(string username)
{
    GenericRequest *loginRequest = new GenericRequest();
    loginRequest->setCommand(LOGIN);
    loginRequest->setUsername(username);
    return loginRequest;
}

GenericRequest *SpecializedRequest::getLogoutRequest(string username)
{
    GenericRequest* logoutRequest = new GenericRequest();
    logoutRequest->setCommand(LOGOUT);
    logoutRequest->setUsername(username);
    return logoutRequest;
}

GenericRequest *SpecializedRequest::getPairRequest(string sender, string receiver)
{
    GenericRequest *pairRequest = new GenericRequest();
    pairRequest->setCommand(PAIR);
    pairRequest->setSender(sender);
    pairRequest->setReceiver(receiver);
    return pairRequest;
}

GenericRequest *SpecializedRequest::getUnpairRequest(string username)
{
    GenericRequest* unpairRequest = new GenericRequest();
    unpairRequest->setCommand(UNPAIR);
    unpairRequest->setUsername(username);
    return unpairRequest;
}

GenericRequest *SpecializedRequest::getSendNewsRequest(string content, string username)
{
    GenericRequest *sendNewsRequest = new GenericRequest();
    sendNewsRequest->setCommand(SEND_NEWS);
    sendNewsRequest->setUsername(username);
    sendNewsRequest->setContent(content);
    return sendNewsRequest;
}

GenericRequest *SpecializedRequest::getCheckNewsRequest(string username)
{
    GenericRequest *checkNewsRequest = new GenericRequest();
    checkNewsRequest->setCommand(CHECK_NEWS);
    checkNewsRequest->setUsername(username);
    return checkNewsRequest;
}
