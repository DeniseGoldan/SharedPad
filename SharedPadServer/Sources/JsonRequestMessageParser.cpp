#include "../Headers/JsonRequestMessageParser.h"
#include "../include/rapidjson/document.h"
#include "../Headers/NamespaceSPP.h"
#include "../Headers/ErrorHandler.h"

JsonRequestMessageParser::JsonRequestMessageParser() {
}

rapidjson::Document *JsonRequestMessageParser::parseJsonMessage(const char *jsonMessage) {
    using namespace rapidjson;
    Document document;

    // Parse jsonMessage (string) into DOM
    if (document.Parse(jsonMessage).HasParseError()) {
        ErrorHandler::handleErrorWithoutExit("Parsing the json request into a document resulted in a error.\n");
        return nullptr;
    }

    // Ckeck structure of jsonMessage sent by client. Must contain COMMAND and ARGUMENTS
    if (!document.HasMember(COMMAND)
        || !document.HasMember(ARGUMENTS)) {
        ErrorHandler::handleErrorWithoutExit("The document has the COMMAND member or ARGUMENTS missing.\n");
        return nullptr;
    }

    if (document[COMMAND].IsNull()) {
        ErrorHandler::handleErrorWithoutExit("The document has the COMMAND member equal to null.\n");
        return nullptr;
    }

    if (!document.IsObject() || !document[COMMAND].IsString() || !document[ARGUMENTS].IsObject()) {
        ErrorHandler::handleErrorWithoutExit("The document has got a mismatch between the expected type and the actual type of the blocks.\n");
        return nullptr;
    }

    if (!JsonRequestMessageParser::argumentsCorrespondToCommand(document[COMMAND].GetString(), document)) {
        ErrorHandler::handleErrorWithoutExit("The provided arguments do not correspond to the COMMAND.\n");
        return nullptr;
    }

    Document *newDocument = new Document();
    // The value from source(document) is moved, not copied, to destination (newDocument)
    newDocument->CopyFrom(document, newDocument->GetAllocator());
    return newDocument;
}

bool JsonRequestMessageParser::argumentsCorrespondToCommand(const char *command, const rapidjson::Document &document) {
    if (command == LOGIN){
        if (!document[ARGUMENTS].HasMember(USERNAME) || !document[ARGUMENTS][USERNAME].IsString()){
            return false;
        }
    }
    return true;
}
