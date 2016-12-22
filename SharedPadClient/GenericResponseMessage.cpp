#include "GenericResponseMessage.h"

GenericResponseMessage::GenericResponseMessage()
{
}

GenericResponseMessage::~GenericResponseMessage()
{

}

int GenericResponseMessage::getCode() const
{
    return code;
}

const std::string &GenericResponseMessage::getCodeDescription() const
{
    return codeDescription;
}

void GenericResponseMessage::setCode(int code)
{
    this->code = code;
}

void GenericResponseMessage::setCodeDescription(const std::string &codeDescription)
{
    this->codeDescription = codeDescription;
}
