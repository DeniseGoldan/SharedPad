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

const string &GenericResponseMessage::getUsername() const
{
    return username;
}

void GenericResponseMessage::setUsername(const string &user)
{
    GenericResponseMessage::username = user;
}

const string &GenericResponseMessage::getIp() const
{
    return ip;
}

void GenericResponseMessage::setIp(const string &ip)
{
    GenericResponseMessage::ip = ip;
}

int GenericResponseMessage::getPort() const
{
    return port;
}

void GenericResponseMessage::setPort(int port)
{
    GenericResponseMessage::port = port;
}

int GenericResponseMessage::getLine() const
{
    return line;
}

void GenericResponseMessage::setLine(int line)
{
    GenericResponseMessage::line = line;
}

const string &GenericResponseMessage::getContent() const
{
    return content;
}

void GenericResponseMessage::setContent(const string &content)
{
    GenericResponseMessage::content = content;
}
