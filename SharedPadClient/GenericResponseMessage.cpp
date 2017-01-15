#include "GenericResponseMessage.h"

GenericResponseMessage::GenericResponseMessage(){}

GenericResponseMessage::~GenericResponseMessage(){}

int GenericResponseMessage::getCode() const
{
    return code;
}

const string &GenericResponseMessage::getCodeDescription() const
{
    return codeDescription;
}

const string &GenericResponseMessage::getSender() const
{
    return sender;
}

const string &GenericResponseMessage::getReceiver() const
{
    return receiver;
}

void GenericResponseMessage::setCode(int code)
{
    this->code = code;
}

void GenericResponseMessage::setCodeDescription(const string &codeDescription)
{
    this->codeDescription = codeDescription;
}

void GenericResponseMessage::setSender(const string &sender)
{
    this->sender = sender;
}

void GenericResponseMessage::setReceiver(const string &receiver)
{
    this->receiver = receiver;
}

const string &GenericResponseMessage::getUsername() const
{
    return username;
}

void GenericResponseMessage::setUsername(const string &user)
{
    this->username = user;
}

const string &GenericResponseMessage::getContent() const
{
    return content;
}

void GenericResponseMessage::setContent(const string &content)
{
    this->content = content;
}
