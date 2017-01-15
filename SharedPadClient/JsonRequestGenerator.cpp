#include "JsonRequestGenerator.h"

JsonRequestGenerator::JsonRequestGenerator(){}

// Used for LOGIN, LOGOUT, QUERY or CHECK_NEWS
string JsonRequestGenerator::getJsonLogRequest(const GenericRequest &message)
{
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
    // The ARGUMNETS block may have its own blocks, USERNAME in this case
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

string JsonRequestGenerator::getJsonPairRequest(const GenericRequest &message)
{
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
    // The ARGUMNETS block may have its own blocks, SENDER and RECEIVER in this case
    writer.Key(ARGUMENTS);
    writer.StartObject();
    writer.Key(SENDER);
    if (message.getSender().empty())
    {
        writer.Null();
    }
    else
    {
        writer.String(message.getSender().c_str());
    }
    writer.Key(RECEIVER);
    if (message.getReceiver().empty())
    {
        writer.Null();
    }
    else
    {
        writer.String(message.getReceiver().c_str());
    }
    // Ending the ARGUMENTS block
    writer.EndObject();
    writer.EndObject();
    return buffer.GetString();
}

string JsonRequestGenerator::getJsonSyncRequest(const GenericRequest &message)
{
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
    // The ARGUMNETS block may have its own blocks, SENDER and RECEIVER in this case
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
    writer.Key(CONTENT);
    if (message.getContent().empty())
    {
        writer.String(" ");
    }
    else
    {
        writer.String(message.getContent().c_str());
    }
    // Ending the ARGUMENTS block
    writer.EndObject();
    writer.EndObject();
    return buffer.GetString();
}













