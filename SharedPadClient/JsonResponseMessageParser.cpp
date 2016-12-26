#include "JsonResponseMessageParser.h"
#include "NamespaceSPP.h"
#include "ErrorHandler.h"

JsonResponseMessageParser::JsonResponseMessageParser(){}

rapidjson::Document *JsonResponseMessageParser::parseJsonMessage(const char *jsonMessage) {
    using namespace rapidjson;
    Document document;

    // Parse jsonMessage (string) into DOM
    if (document.Parse(jsonMessage).HasParseError()) {
        ErrorHandler::handleErrorWithoutExit("Parsing the json response into a document resulted in a error.\n");
        return nullptr;
    }
    // Ckeck structure of jsonMessage sent by server
    if (!document.HasMember(CODE) || !document.HasMember(CODE_DESCRIPTION)) {
        ErrorHandler::handleErrorWithoutExit("The document has a member missing (either the CODE block, or the CODE_DESCRIPTION block).\n");
        return nullptr;
    }

    if (document[CODE].IsNull()||document[CODE_DESCRIPTION].IsNull()) {
        ErrorHandler::handleErrorWithoutExit("The document has a null member.\n");
        return nullptr;
    }

    if (!document.IsObject() || !document[CODE].IsInt() || !document[CODE_DESCRIPTION].IsString()) {
        ErrorHandler::handleErrorWithoutExit("The document has got a mismatch between the expected type and the actual type of the blocks.\n");
        return nullptr;
    }

    if (!JsonResponseMessageParser::codeCorrespondsToContent(document[CODE].GetInt(),document)){
        ErrorHandler::handleErrorWithoutExit("CODE does not correspond to content.\n");
        return nullptr;
    }

    Document *newDocument = new Document();
    // The value from source(document) is moved, not copied, to destination (newDocument)
    newDocument->CopyFrom(document, newDocument->GetAllocator());
    return newDocument;
}

bool JsonResponseMessageParser::codeCorrespondsToContent(int code, const rapidjson::Document &document){
    if (code == LOGIN_APPROVED_CODE || code == LOGIN_FAILED_CODE) {
        if (!document.HasMember(USERNAME)) {
            return false;
        }
    }
    return true;
}
