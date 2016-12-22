#include "JsonResponseMessageParser.h"
#include "NamespaceSPP.h"

JsonResponseMessageParser::JsonResponseMessageParser(){

}

rapidjson::Document *JsonResponseMessageParser::parseJsonMessage(const char *jsonMessage) {
    using namespace rapidjson;
    Document document;

    // Parse jsonMessage (string) into DOM
    if (document.Parse(jsonMessage).HasParseError()) {
        return nullptr;
    }
    // Ckeck structure of jsonMessage sent by server
    if (!document.HasMember(CODE)
        || !document.HasMember(CODE_DESCRIPTION)) {
        return nullptr;
    }
    if (document[CODE].IsNull()||document[CODE_DESCRIPTION].IsNull()) {
        return nullptr;
    }

    if (!document[CODE].IsInt()
        || !document[CODE_DESCRIPTION].IsString()
        || !document.IsObject()) {
        return nullptr;
    }

    if (!JsonResponseMessageParser::codeCorrespondsToContent(document[CODE].GetInt(),document)){
        return nullptr;
    }

    Document *newDocument = new Document;
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
