#include "../Headers/JsonResponseMessageGenerator.h"

JsonResponseMessageGenerator::JsonResponseMessageGenerator(){}

string JsonResponseMessageGenerator::getJsonBasicResponseMessage(const GenericResponseMessage &response)
{
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);

    writer.StartObject();

    writer.Key(CODE);
    if (-1 == response.getCode())
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

    writer.Key(CLIENT_PORT);
    if (-1 == response.getPort())
    {
        writer.Null();
    }
    else
    {
        writer.Int(response.getPort());
    }

    writer.Key(CLIENT_IP_ADDRESS);
    if (response.getIp().empty())
    {
        writer.Null();
    }
    else
    {
        writer.String(response.getIp().c_str());
    }

    writer.Key(USERNAME);
    if (response.getUsername().empty())
    {
        writer.Null();
    }
    else
    {
        writer.String(response.getUsername().c_str());
    }

    writer.Key(SENDER);
    if (response.getSender().empty())
    {
        writer.Null();
    }
    else
    {
        writer.String(response.getSender().c_str());
    }

    writer.Key(RECEIVER);
    if (response.getReceiver().empty())
    {
        writer.Null();
    }
    else
    {
        writer.String(response.getReceiver().c_str());
    }

    writer.Key(CONTENT);
    if (response.getContent().empty())
    {
        writer.Null();
    }
    else
    {
        writer.String(response.getContent().c_str());
    }

    writer.EndObject();
    return buffer.GetString();
}