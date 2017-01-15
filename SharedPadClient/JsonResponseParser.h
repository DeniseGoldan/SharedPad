#ifndef CLIENT_JSON_RESPONSE_GENERATOR_H
#define CLIENT_JSON_RESPONSE_GENERATOR_H

#include <string.h>

#include "rapidjson/document.h"
#include "spdlog/spdlog.h"

#include "StatusCodesAndDescriptions.h"

using namespace rapidjson;
using namespace std;


class JsonResponseParser
{
public:
    JsonResponseParser();
    static Document * parseJsonMessage(const char * jsonMessage);

private:
    static bool codeMatchesContent(int code, const Document &document);
};

#endif // CLIENT_JSON_RESPONSE_GENERATOR_H
