#include "../Headers/JsonResponseMessageGenerator.h"
#include "../Headers/NamespaceSPP.h"

JsonResponseMessageGenerator::JsonResponseMessageGenerator()
{
}

std::string JsonResponseMessageGenerator::getJsonBasicResponseMessage(const GenericResponseMessage &response)
{
    using namespace rapidjson;

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

std::string JsonResponseMessageGenerator::getJsonLoginResponseMessage(const LoginResponseMessage &loginMessage)
{
    using namespace rapidjson;

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);

    writer.StartObject();

    writer.Key(CODE);
    if (loginMessage.getCode() == -1)
    {
        writer.Null();
    } else
    {
        writer.Int(loginMessage.getCode());
    }

    writer.Key(CODE_DESCRIPTION);
    if (loginMessage.getCodeDescription().empty())
    {
        writer.Null();
    } else
    {
        writer.String(loginMessage.getCodeDescription().c_str());
    }

    writer.Key(USERNAME);
    if (loginMessage.getUsername().empty())
    {
        writer.Null();
    } else
    {
        writer.String(loginMessage.getUsername().c_str());
    }

    writer.Key(CLIENT_IP_ADDRESS);
    if (loginMessage.getIpAddress().empty())
    {
        writer.Null();
    } else
    {
        writer.String(loginMessage.getIpAddress().c_str());
    }

    writer.Key(CLIENT_PORT);
    if (loginMessage.getPort() == -1)
    {
        writer.Null();
    } else
    {
        writer.Int(loginMessage.getPort());
    }

    writer.EndObject();

    return buffer.GetString();
}
