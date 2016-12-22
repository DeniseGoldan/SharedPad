#include "JsonRequestMessageGenerator.h"
#include "NamespaceSPP.h"

JsonRequestMessageGenerator::JsonRequestMessageGenerator()
{

}

std::string JsonRequestMessageGenerator::getJsonLoginRequestMessage(const GenericRequestMessage &message){
    using namespace rapidjson;

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);

    writer.StartObject();

    writer.Key(COMMAND);
    if (message.getCommand().empty()) {
        writer.Null();
    } else {
        writer.String(message.getCommand().c_str());
    }
    writer.Key(ARGUMENTS);
    writer.StartObject();
    writer.Key(USERNAME);

    if (message.getUsername().empty()) {
        writer.Null();
    } else {
        writer.String(message.getUsername().c_str());
    }

    writer.EndObject();

    writer.EndObject();
    return buffer.GetString();
}
