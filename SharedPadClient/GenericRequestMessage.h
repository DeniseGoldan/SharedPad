#ifndef GENERICREQUESTMESSAGE_H
#define GENERICREQUESTMESSAGE_H

#include <string>

using namespace std;

class GenericRequestMessage
{
public:
    GenericRequestMessage();

    ~GenericRequestMessage();

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

#endif  // GENERICREQUESTMESSAGE_H
