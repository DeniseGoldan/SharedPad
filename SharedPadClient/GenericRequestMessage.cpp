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

const string &GenericRequestMessage::getIp() const
{
    return ip;
}

void GenericRequestMessage::setIp(const string &ip)
{
    this->ip = ip;
}

int GenericRequestMessage::getPort() const
{
    return port;
}

void GenericRequestMessage::setPort(int port)
{
    this->port = port;
}

int GenericRequestMessage::getLine() const
{
    return line;
}

void GenericRequestMessage::setLine(int line)
{
    this->line = line;
}

const string &GenericRequestMessage::getContent() const
{
    return content;
}

void GenericRequestMessage::setContent(const string &content)
{
    this->content = content;
}
