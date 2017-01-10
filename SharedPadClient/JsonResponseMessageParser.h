#ifndef JSONRESPONSEMESSAGEPARSER_H
#define JSONRESPONSEMESSAGEPARSER_H

#include <string.h>

#include "rapidjson/document.h"
#include "spdlog/spdlog.h"

#include "StatusCodesAndDescriptions.h"

using namespace rapidjson;


class JsonResponseMessageParser
{
public:
    JsonResponseMessageParser();
    static rapidjson::Document * parseJsonMessage(const char * jsonMessage);

private:
    static bool codeCorrespondsToContent(int code, const rapidjson::Document &document);
};

#endif // JSONRESPONSEMESSAGEPARSER_H
