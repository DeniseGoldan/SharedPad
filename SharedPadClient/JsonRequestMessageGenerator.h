#ifndef JSONMESSAGEGENERATOR_H
#define JSONMESSAGEGENERATOR_H

#include <string>

#include "rapidjson/writer.h"
#include "spdlog/spdlog.h"

#include "GenericRequestMessage.h"
#include "StatusCodesAndDescriptions.h"

using namespace rapidjson;
using namespace std;

class JsonRequestMessageGenerator {
public:
    JsonRequestMessageGenerator();
    static string getJsonLogRequestMessage(const GenericRequestMessage &message);
    static string getJsonPairRequestMessage(const GenericRequestMessage &message);
private:
};

#endif // JSONMESSAGEGENERATOR_H


