#include "../Headers/GenericResponseMessage.h"

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
    GenericResponseMessage::sender = sender;
}

void GenericResponseMessage::setReceiver(const string &receiver)
{
    GenericResponseMessage::receiver = receiver;
}
