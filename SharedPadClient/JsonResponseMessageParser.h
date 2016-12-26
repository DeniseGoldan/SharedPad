#ifndef JSONRESPONSEMESSAGEPARSER_H
#define JSONRESPONSEMESSAGEPARSER_H
#include "spdlog/spdlog.h"
#include "rapidjson/document.h"

class JsonResponseMessageParser
{
public:
    JsonResponseMessageParser();
    static rapidjson::Document * parseJsonMessage(const char * jsonMessage);

private:
    static bool codeCorrespondsToContent(int code, const rapidjson::Document &document);
};

#endif // JSONRESPONSEMESSAGEPARSER_H
