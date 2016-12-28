#include "../../Headers/JsonMessageContainers/LoginResponseMessage.h"

LoginResponseMessage::LoginResponseMessage()
{

}

LoginResponseMessage::~LoginResponseMessage()
{

}

const std::string &LoginResponseMessage::getUsername() const
{
    return username;
}

const std::string &LoginResponseMessage::getIpAddress() const
{
    return ipAddress;
}

int LoginResponseMessage::getPort() const
{
    return port;
}

void LoginResponseMessage::setUsername(const std::string &username)
{
    this->username = username;
}

void LoginResponseMessage::setIpAddress(const std::string &ipAddress)
{
    this->ipAddress = ipAddress;
}

void LoginResponseMessage::setPort(int port)
{
    this->port = port;
}
