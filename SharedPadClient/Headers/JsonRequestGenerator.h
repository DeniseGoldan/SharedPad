#ifndef CLIENT_JSON_REQUEST_GENERATOR_H
#define CLIENT_JSON_REQUEST_GENERATOR_H

#include <string>

#include "rapidjson/writer.h"
#include "spdlog/spdlog.h"

#include "GenericRequest.h"
#include "StatusCodesAndDescriptions.h"

using namespace rapidjson;
using namespace std;

class JsonRequestGenerator {
public:
    JsonRequestGenerator();
    static string getJsonLogRequest(const GenericRequest &message);
    static string getJsonPairRequest(const GenericRequest &message);
    static string getJsonSyncRequest(const GenericRequest &message);
};

#endif // CLIENT_JSON_REQUEST_GENERATOR_H
