#ifndef SHAREPADSERVER_CREATEJSONMESSAGE_H
#define SHAREPADSERVER_CREATEJSONMESSAGE_H

#include <string>

#include "../include/rapidjson/writer.h"

#include "StatusCodesAndDescriptions.h"
#include "JsonMessageContainers/GenericResponseMessage.h"

using namespace rapidjson;

class JsonResponseMessageGenerator {
public:
    static std::string getJsonBasicResponseMessage(const GenericResponseMessage &response);

private:
    JsonResponseMessageGenerator();
};

#endif //SHAREPADSERVER_CREATEJSONMESSAGE_H
