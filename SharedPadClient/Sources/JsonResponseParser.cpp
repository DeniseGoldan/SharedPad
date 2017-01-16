#include "JsonResponseParser.h"

auto parseJsonMessagelogger = spdlog::stdout_color_mt("parseJsonMessagelogger_logger");
auto codeCorrespondsToContentlogger = spdlog::stdout_color_mt("codeCorrespondsToContent_logger");

JsonResponseParser::JsonResponseParser(){}

Document *JsonResponseParser::parseJson(const char *jsonMessage)
{
    //This variable will exist only inside this scope, in the stack.
    Document stackDocument;
    // Parse jsonMessage (string) into DOM.
    if (stackDocument.Parse(jsonMessage).HasParseError())
    {
        parseJsonMessagelogger->warn("Parsing the json response into a document resulted in a error.");
        return nullptr;
    }
    // Ckeck structure of jsonMessage sent by server.
    if (!stackDocument.HasMember(CODE) || !stackDocument.HasMember(CODE_DESCRIPTION))
    {
        parseJsonMessagelogger->warn("The document has a member missing (either the CODE block, or the CODE_DESCRIPTION block).");
        return nullptr;
    }
    if (stackDocument[CODE].IsNull())
    {
        parseJsonMessagelogger->warn("The document has a null member(CODE).");
        return nullptr;
    }
    if (!stackDocument.IsObject() || !stackDocument[CODE].IsInt() || !stackDocument[CODE_DESCRIPTION].IsString())
    {
        parseJsonMessagelogger->warn("The document has got a mismatch between the expected type and the actual type of the blocks.");
        return nullptr;
    }
    if (!JsonResponseParser::codeMatchesContent(stackDocument[CODE].GetInt(),stackDocument))
    {
        parseJsonMessagelogger->warn("CODE does not correspond to content.");
        return nullptr;
    }
    /*
    * Create a variable inside the heap, not the stack.
    * The value from source(document) is moved, not copied, to destination (heapDocument).
    */
    Document *heapDocument = new Document();
    heapDocument->CopyFrom(stackDocument, heapDocument->GetAllocator());
    return heapDocument;
}

bool JsonResponseParser::codeMatchesContent(int code, const Document &document)
{
    if (code == LOGIN_APPROVED_CODE )
    {
        std::string result = document[CODE_DESCRIPTION].GetString();
        if (strcmp(result.c_str(), LOGIN_APPROVED) !=0)
        {
            return false;
        }
    }
    if (code == LOGOUT_APPROVED_CODE )
    {
        string result = document[CODE_DESCRIPTION].GetString();
        if (strcmp(result.c_str(), LOGIN_FAILED) !=0)
        {
            return false;
        }
    }
    if (code == LOGIN_FAILED_CODE )
    {
        string result = document[CODE_DESCRIPTION].GetString();
        if (strcmp(result.c_str(), LOGOUT_APPROVED) !=0)
        {
            return false;
        }
    }
    if (code == INVITED_YOURSELF_CODE )
    {
        string result = document[CODE_DESCRIPTION].GetString();
        if (strcmp(result.c_str(), INVITED_YOURSELF) !=0)
        {
            return false;
        }
    }
    if (code == USER_NOT_LOGGED_IN_CODE )
    {
        string result = document[CODE_DESCRIPTION].GetString();
        if (strcmp(result.c_str(), USER_NOT_LOGGED_IN) !=0)
        {
            return false;
        }
    }
    if (code == ALREADY_PAIRED_CODE )
    {
        string result = document[CODE_DESCRIPTION].GetString();
        if (strcmp(result.c_str(), ALREADY_PAIRED) !=0)
        {
            return false;
        }
    }
    if (code == YOU_ARE_SINGLE_CODE )
    {
        string result = document[CODE_DESCRIPTION].GetString();
        if (strcmp(result.c_str(), YOU_ARE_SINGLE) !=0)
        {
            return false;
        }
    }
    if (code == SENT_NEWS_TO_PEER_CODE )
    {
        string result = document[CODE_DESCRIPTION].GetString();
        if (strcmp(result.c_str(), SENT_NEWS_TO_PEER) !=0)
        {
            return false;
        }
    }
    if (code == JSON_PARSING_FAILED_CODE )
    {
        string result = document[CODE_DESCRIPTION].GetString();
        if (strcmp(result.c_str(), JSON_PARSING_FAILED) !=0)
        {
            return false;
        }
    }
    return true;
}
