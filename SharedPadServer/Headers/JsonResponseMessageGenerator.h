#ifndef SHAREDPADSERVER_CREATEJSONMESSAGE_H
#define SHAREDPADSERVER_CREATEJSONMESSAGE_H

#include <string>

#include "../include/rapidjson/writer.h"

#include "StatusCodesAndDescriptions.h"
#include "GenericResponseMessage.h"

using namespace rapidjson;
using namespace std;

class JsonResponseMessageGenerator {
public:
    static string getJsonBasicResponseMessage(const GenericResponseMessage &response);
private:
    JsonResponseMessageGenerator();
};

#endif //SHAREDPADSERVER_CREATEJSONMESSAGE_H
