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
    int line = -1;
    string content;
};


#endif //SHAREDPADSERVER_GENERICMESSAGE_H
