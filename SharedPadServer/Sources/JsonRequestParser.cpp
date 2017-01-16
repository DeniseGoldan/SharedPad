#include "../Headers/JsonRequestParser.h"

auto parseJsonMessage_logger = spdlog::stdout_color_mt("parseJsonMessage_logger");

JsonRequestParser::JsonRequestParser(){}

Document *JsonRequestParser::parseJson(const char *jsonMessage)
{
    //This variable will exist only inside this scope, in the stack.
    Document stackDocument;
    // Parse jsonMessage (string) into DOM
    if (stackDocument.Parse(jsonMessage).HasParseError())
    {
        parseJsonMessage_logger->warn("Parsing the json request into a stackDocument resulted in a error.");
        return nullptr;
    }
    // Check structure of jsonMessage sent by client. Must contain COMMAND and ARGUMENTS
    if (!stackDocument.HasMember(COMMAND) || !stackDocument.HasMember(ARGUMENTS))
    {
        parseJsonMessage_logger->warn("The stackDocument has the COMMAND or ARGUMENTS missing.");
        return nullptr;
    }
    if (!stackDocument.IsObject() || !stackDocument[COMMAND].IsString() || !stackDocument[ARGUMENTS].IsObject())
    {
        parseJsonMessage_logger->warn("The stackDocument has got a mismatch between the expected type and the actual type of the blocks.");
        return nullptr;
    }
    if (stackDocument[COMMAND].IsNull())
    {
        parseJsonMessage_logger->warn("The stackDocument has the COMMAND member equal to null.");
        return nullptr;
    }
    if (!JsonRequestParser::argumentsMatchCommand(stackDocument[COMMAND].GetString(), stackDocument))
    {
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

bool JsonRequestParser::argumentsMatchCommand(const char *command, const Document &document)
{
    // LOGIN, LOGOUT, HEARTBEAT, CHECK_NEWS
    if (command == LOGIN
        || command == LOGOUT
        || command == UNPAIR
        || command == HEARTBEAT
        || command == CHECK_NEWS)
    {
        if (!document[ARGUMENTS].HasMember(USERNAME) || !document[ARGUMENTS][USERNAME].IsString())
        {
            return false;
        }
    }
    // PAIR
    if (command == PAIR)
    {
        if (!document[ARGUMENTS].HasMember(SENDER) || !document[ARGUMENTS][SENDER].IsString())
        {
            return false;
        }
        if (!document[ARGUMENTS].HasMember(RECEIVER) || !document[ARGUMENTS][RECEIVER].IsString())
        {
            return false;
        }
    }
    // SEND_NEWS
    if (command == SEND_NEWS)
    {
        if (!document[ARGUMENTS].HasMember(USERNAME) || !document[ARGUMENTS][USERNAME].IsString())
        {
            return false;
        }
        if (!document[ARGUMENTS].HasMember(CONTENT) || !document[ARGUMENTS][CONTENT].IsString())
        {
            return false;
        }
    }
    return true;
}
