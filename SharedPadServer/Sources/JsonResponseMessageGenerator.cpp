#include "../Headers/JsonResponseMessageGenerator.h"

JsonResponseMessageGenerator::JsonResponseMessageGenerator(){}

std::string JsonResponseMessageGenerator::getJsonBasicResponseMessage(const GenericResponseMessage &response)
{
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);

    writer.StartObject();

    writer.Key(CODE);
    if (response.getCode() == -1)
    {
        writer.Null();
    } else
    {
        writer.Int(response.getCode());
    }

    writer.Key(CODE_DESCRIPTION);
    if (response.getCodeDescription().empty())
    {
        writer.Null();
    } else
    {
        writer.String(response.getCodeDescription().c_str());
    }
    writer.EndObject();

    return buffer.GetString();
}
