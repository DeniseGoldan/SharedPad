#ifndef JSONRESPONSEMESSAGEPARSER_H
#define JSONRESPONSEMESSAGEPARSER_H

#include <string.h>

#include "rapidjson/document.h"
#include "spdlog/spdlog.h"

#include "StatusCodesAndDescriptions.h"

using namespace rapidjson;
using namespace std;


class JsonResponseMessageParser
{
public:
    JsonResponseMessageParser();
    static Document * parseJsonMessage(const char * jsonMessage);

private:
    static bool codeCorrespondsToContent(int code, const Document &document);
};

#endif // JSONRESPONSEMESSAGEPARSER_H
