#include "../Headers/JsonRequestMessageParser.h"

auto parseJsonMessage_logger = spdlog::stdout_color_mt("parseJsonMessage_logger");

JsonRequestMessageParser::JsonRequestMessageParser() {}

Document *JsonRequestMessageParser::parseJsonMessage(const char *jsonMessage) {
    //This variable will exist only inside this scope, in the stack.
    Document stackDocument;

    // Parse jsonMessage (string) into DOM
    if (stackDocument.Parse(jsonMessage).HasParseError()) {
        parseJsonMessage_logger->warn("Parsing the json request into a stackDocument resulted in a error.");
        return nullptr;
    }

    // Check structure of jsonMessage sent by client. Must contain COMMAND and ARGUMENTS
    if (!stackDocument.HasMember(COMMAND) || !stackDocument.HasMember(ARGUMENTS)) {
        parseJsonMessage_logger->warn("The stackDocument has the COMMAND or ARGUMENTS missing.");
        return nullptr;
    }

    if (stackDocument[COMMAND].IsNull()) {
        parseJsonMessage_logger->warn("The stackDocument has the COMMAND member equal to null.");
        return nullptr;
    }

    if (!stackDocument.IsObject() || !stackDocument[COMMAND].IsString() || !stackDocument[ARGUMENTS].IsObject()) {
        parseJsonMessage_logger->warn("The stackDocument has got a mismatch between the expected type and the actual type of the blocks.");
        return nullptr;
    }

    if (!JsonRequestMessageParser::argumentsCorrespondToCommand(stackDocument[COMMAND].GetString(), stackDocument)) {
        parseJsonMessage_logger->warn("The provided arguments do not correspond to the COMMAND.");
        return nullptr;
    }

    /*
     * Create a variable inside the heap, not the stack.
     * The value from source(stackDocument) is moved, not copied, to destination (heapDocument).
     */
    Document *heapDocument = new Document();
    heapDocument->CopyFrom(stackDocument, heapDocument->GetAllocator());
    return heapDocument;
}

bool JsonRequestMessageParser::argumentsCorrespondToCommand(const char *command, const Document &document) {

    // LOGIN or LOGOUT or UPDATE_CONN_TEST
    if (command == LOGIN || command == LOGOUT || command == UPDATE_CONN_TEST){
        if (!document[ARGUMENTS].HasMember(USERNAME) || !document[ARGUMENTS][USERNAME].IsString()){
            return false;
        }
    }
    return true;
}
