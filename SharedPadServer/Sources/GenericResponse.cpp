#include "../Headers/GenericResponse.h"

GenericResponse::GenericResponse(){}

GenericResponse::~GenericResponse(){}

int GenericResponse::getCode() const
{
    return code;
}

const string &GenericResponse::getCodeDescription() const
{
    return codeDescription;
}

const string &GenericResponse::getSender() const
{
    return sender;
}

const string &GenericResponse::getReceiver() const
{
    return receiver;
}

void GenericResponse::setCode(int code)
{
    this->code = code;
}

void GenericResponse::setCodeDescription(const string &codeDescription)
{
    this->codeDescription = codeDescription;
}

void GenericResponse::setSender(const string &sender)
{
    this->sender = sender;
}

void GenericResponse::setReceiver(const string &receiver)
{
    this->receiver = receiver;
}

const string &GenericResponse::getUsername() const
{
    return username;
}

void GenericResponse::setUsername(const string &user)
{
    this->username = user;
}

const string &GenericResponse::getContent() const
{
    return content;
}

void GenericResponse::setContent(const string &content)
{
    this->content = content;
}
