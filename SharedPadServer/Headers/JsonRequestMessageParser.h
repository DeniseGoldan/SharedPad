#ifndef SHAREDPADSERVER_JSONMESSAGEPARSER_H
#define SHAREDPADSERVER_JSONMESSAGEPARSER_H

#include "../include/rapidjson/document.h"
#include "../include/spdlog/spdlog.h"

#include "StatusCodesAndDescriptions.h"

using namespace rapidjson;

class JsonRequestMessageParser {
public:
    static Document *parseJsonMessage(const char *jsonMessage);

private:
    JsonRequestMessageParser();

    static bool argumentsCorrespondToCommand(const char *command, const Document &document);
};

#endif //SHAREDPADSERVER_JSONMESSAGEPARSER_H
