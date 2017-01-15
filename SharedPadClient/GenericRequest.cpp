#include "GenericRequest.h"

GenericRequest::GenericRequest(){}

GenericRequest::~GenericRequest(){}

const string &GenericRequest::getCommand() const
{
    return command;
}

const string &GenericRequest::getUsername() const
{
    return username;
}

const string &GenericRequest::getSender() const
{
    return sender;
}

const string &GenericRequest::getReceiver() const
{
    return receiver;
}

void GenericRequest::setCommand(const string &command)
{
    this->command = command;
}

void GenericRequest::setUsername(const string &username)
{
    this->username = username;
}

void GenericRequest::setSender(const string &sender)
{
    this->sender = sender;
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
