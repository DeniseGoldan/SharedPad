#include "GenericRequest.h"

GenericRequest::GenericRequest(){}

GenericRequest::~GenericRequest(){}

const string &GenericRequest::getCommand() const
{
    return command;
}

void GenericRequest::setCommand(const string &command)
{
    this->command = command;
}

const string &GenericRequest::getUsername() const
{
    return username;
}

void GenericRequest::setUsername(const string &username)
{
    this->username = username;
}

const string &GenericRequest::getSender() const
{
    return sender;
}

void GenericRequest::setSender(const string &sender)
{
    this->sender = sender;
}

const string &GenericRequest::getReceiver() const
{
    return receiver;
}

void GenericRequest::setReceiver(const string &receiver)
{
    this->receiver = receiver;
}

const string &GenericRequest::getContent() const
{
    return content;
}

void GenericRequest::setContent(const string &content)
{
    this->content = content;
}
