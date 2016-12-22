#ifndef SHAREPADSERVER_CREATEJSONMESSAGE_H
#define SHAREPADSERVER_CREATEJSONMESSAGE_H

#include <string>
#include "../include/rapidjson/writer.h"
#include "JsonMessageContainers/LoginResponseMessage.h"

class JsonResponseMessageGenerator {
public:
    static std::string getJsonBasicResponseMessage(const GenericResponseMessage &response);

    static std::string getJsonLoginResponseMessage(const LoginResponseMessage &loginMessage);

private:
    JsonResponseMessageGenerator();
};

#endif //SHAREPADSERVER_CREATEJSONMESSAGE_H
