#include "JsonRequestMessageGenerator.h"
#include "NamespaceSPP.h"

JsonRequestMessageGenerator::JsonRequestMessageGenerator(){}

std::string JsonRequestMessageGenerator::getJsonLoginRequestMessage(const GenericRequestMessage &message){
    using namespace rapidjson;

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);

    writer.StartObject();
    // This object contains a COMMAND and its ARGUMENTS, organized into 2 blocks
    writer.Key(COMMAND);
    if (message.getCommand().empty()) {
        writer.Null();
    } else {
        writer.String(message.getCommand().c_str());
    }
    // The ARGUMNETS block may have its own blocks
    writer.Key(ARGUMENTS);
    writer.StartObject();
    writer.Key(USERNAME);
    if (message.getUsername().empty()) {
        writer.Null();
    } else {
        writer.String(message.getUsername().c_str());
    }
    writer.EndObject();
    // Ending the ARGUMENTS block
    writer.EndObject();
    return buffer.GetString();
}
