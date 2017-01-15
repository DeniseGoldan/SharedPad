#ifndef SERVER_JSON_REQUEST_PARSER
#define SERVER_JSON_REQUEST_PARSER

#include "../include/rapidjson/document.h"
#include "../include/spdlog/spdlog.h"

#include "StatusCodesAndDescriptions.h"

using namespace rapidjson;

class JsonRequestParser
{
public:
    static Document *parseJson(const char *json);

private:
    JsonRequestParser();

    static bool argumentsCorrespondToCommand(const char *command, const Document &document);
};

#endif //SERVER_JSON_REQUEST_PARSER
