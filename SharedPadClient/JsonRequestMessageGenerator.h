#ifndef JSONMESSAGEGENERATOR_H
#define JSONMESSAGEGENERATOR_H

#include <string>

#include "rapidjson/writer.h"
#include "spdlog/spdlog.h"

#include "GenericRequestMessage.h"
#include "NamespaceSPP.h"

using namespace rapidjson;

class JsonRequestMessageGenerator {
public:
    JsonRequestMessageGenerator();
    static std::string getJsonLoginRequestMessage(const GenericRequestMessage &message);
private:
};

#endif // JSONMESSAGEGENERATOR_H


