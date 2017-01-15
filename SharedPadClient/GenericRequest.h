#ifndef CLIENT_GENERIC_REQUEST_H
#define CLIENT_GENERIC_REQUEST_H

#include <string>

using namespace std;

class GenericRequest
{
public:
    GenericRequest();

    ~GenericRequest();

    void setCommand(const string &command);

    const string &getCommand() const;

    void setUsername(const string &username);

    const string &getUsername() const;

    const string &getSender() const;

    void setSender(const string &sender);

    const string &getReceiver() const;

    void setReceiver(const string &receiver);

    const string &getContent() const;

    void setContent(const string &content);


private:

    string username;

    string command;

    string sender;

    string receiver;

    string content;

};

#endif  // CLIENT_GENERIC_REQUEST_H
