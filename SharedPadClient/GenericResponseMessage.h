#ifndef SHAREDPADSERVER_GENERICMESSAGE_H
#define SHAREDPADSERVER_GENERICMESSAGE_H

#include <string>

using namespace std;

class GenericResponseMessage
{
public:
    GenericResponseMessage();

    virtual ~GenericResponseMessage();

    int getCode() const;

    const string &getCodeDescription() const;

    void setCode(int code);

    void setCodeDescription(const string &codeDescription);

    const string &getSender() const;

    void setSender(const string &sender);

    const string &getReceiver() const;

    void setReceiver(const string &receiver);

    const string &getUsername() const;

    void setUsername(const string &user);

    const string &getIp() const;

    void setIp(const string &ip);

    int getPort() const;

    void setPort(int port);

    int getLine() const;

    void setLine(int line);

    const string &getContent() const;

    void setContent(const string &content);

private:
    int code = -1;
    string codeDescription;
    string username;
    string sender;
    string receiver;
    string ip;
    int port  = -1;
    int line = -1;
    string content;
};


#endif //SHAREDPADSERVER_GENERICMESSAGE_H
