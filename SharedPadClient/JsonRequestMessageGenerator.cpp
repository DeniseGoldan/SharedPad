#include "JsonRequestMessageGenerator.h"

JsonRequestMessageGenerator::JsonRequestMessageGenerator(){}

// Used for LOGIN, LOGOUT or QUERY
std::string JsonRequestMessageGenerator::getJsonLogRequestMessage(const GenericRequestMessage &message){

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    writer.StartObject();

    // This object contains a COMMAND and its ARGUMENTS, organized into 2 distinct blocks
    writer.Key(COMMAND);
    if (message.getCommand().empty())
    {
        writer.Null();
    }
    else
    {
        writer.String(message.getCommand().c_str());
    }

    // The ARGUMNETS block may have its own blocks
    writer.Key(ARGUMENTS);
    writer.StartObject();
    writer.Key(USERNAME);
    if (message.getUsername().empty())
    {
        writer.Null();
    }
    else
    {
        writer.String(message.getUsername().c_str());
    }

    // Ending the ARGUMENTS block
    writer.EndObject();

    writer.EndObject();
    return buffer.GetString();
}
