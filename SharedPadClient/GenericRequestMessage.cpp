#include "GenericRequestMessage.h"

GenericRequestMessage::GenericRequestMessage()
{

}

GenericRequestMessage::~GenericRequestMessage()
{

}

const std::string &GenericRequestMessage::getUsername() const
{
    return username;
}

const std::string &GenericRequestMessage::getCommand() const
{
    return command;
}

void GenericRequestMessage::setUsername(const std::string &username)
{
    this->username = username;
}

void GenericRequestMessage::setCommand(const std::string &command)
{
    this->command = command;
}


