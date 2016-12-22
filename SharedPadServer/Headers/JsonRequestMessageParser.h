#ifndef SHAREPADSERVER_JSONMESSAGEPARSER_H
#define SHAREPADSERVER_JSONMESSAGEPARSER_H

#include "../include/rapidjson/document.h"

class JsonRequestMessageParser {
public:
    static rapidjson::Document *parseJsonMessage(const char *jsonMessage);

private:
    JsonRequestMessageParser();

    static bool argumentsCorrespondToCommand(const char *command, const rapidjson::Document &document);
};

#endif //SHAREPADSERVER_JSONMESSAGEPARSER_H
