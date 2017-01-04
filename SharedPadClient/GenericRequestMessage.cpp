#include "GenericRequestMessage.h"

GenericRequestMessage::GenericRequestMessage(){}

GenericRequestMessage::~GenericRequestMessage(){}

const string &GenericRequestMessage::getCommand() const
{
    return command;
}

const string &GenericRequestMessage::getUsername() const
{
    return username;
}

const string &GenericRequestMessage::getSender() const
{
    return sender;
}

const string &GenericRequestMessage::getReceiver() const
{
    return receiver;
}

void GenericRequestMessage::setCommand(const string &command)
{
    this->command = command;
}

void GenericRequestMessage::setUsername(const string &username)
{
    this->username = username;
}

void GenericRequestMessage::setSender(const string &sender)
{
    this->sender = sender;
}

void GenericRequestMessage::setReceiver(const string &receiver)
{
    this->receiver = receiver;
}




