#include "../Headers/JsonRequestMessageParser.h"
#include "../include/rapidjson/document.h"
#include "../Headers/NamespaceSPP.h"

JsonRequestMessageParser::JsonRequestMessageParser() {
}

rapidjson::Document *JsonRequestMessageParser::parseJsonMessage(const char *jsonMessage) {
    using namespace rapidjson;
    Document document;

    // Parse jsonMessage (string) into DOM
    if (document.Parse(jsonMessage).HasParseError()) {
        return nullptr;
    }
    // Ckeck structure of jsonMessage sent by server. Must contain CODE, CODE_DESCRIPTION and FRAME
    if (!document.HasMember(COMMAND)
        || !document.HasMember(ARGUMENTS)) {
        return nullptr;
    }
    if (document[COMMAND].IsNull()) {
        return nullptr;
    }
    //if (!document[ARGUMENTS].IsNull()) { return nullptr;  }

    if (!document[COMMAND].IsString()
        || !document[ARGUMENTS].IsObject()
        || !document.IsObject()) {
        return nullptr;
    }
    if (!JsonRequestMessageParser::argumentsCorrespondToCommand(document[COMMAND].GetString(), document)) {
        return nullptr;
    }
    Document *newDocument = new Document;
    // The value from source(document) is moved, not copied, to destination (newDocument)
    newDocument->CopyFrom(document, newDocument->GetAllocator());
    return newDocument;
}

bool JsonRequestMessageParser::argumentsCorrespondToCommand(const char *command, const rapidjson::Document &document) {
    if (command == LOGIN){
        if (!document[ARGUMENTS].HasMember(USERNAME)){
            return false;
        }
        if(!document[ARGUMENTS][USERNAME].IsString()){
            return false;
        }
    }
    return true;
}
