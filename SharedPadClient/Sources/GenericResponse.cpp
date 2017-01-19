#include "GenericResponse.h"

GenericResponse::GenericResponse(){}

GenericResponse::~GenericResponse(){}

int GenericResponse::getCode() const
{
    return code;
}

void GenericResponse::setCode(int code)
{
    this->code = code;
}

const string &GenericResponse::getCodeDescription() const
{
    return codeDescription;
}

void GenericResponse::setCodeDescription(const string &codeDescription)
{
    this->codeDescription = codeDescription;
}

const string &GenericResponse::getSender() const
{
    return sender;
}

const string &GenericResponse::getReceiver() const
{
    return receiver;
}

const string &GenericResponse::getUsername() const
{
    return username;
}

const string &GenericResponse::getContent() const
{
    return content;
}
