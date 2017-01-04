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
    void setUsername(const string &username);
    void setSender(const string &sender);
    void setReceiver(const string &receiver);

    const string &getCommand() const;
    const string &getUsername() const;
    const string &getSender() const;
    const string &getReceiver() const;

private:

    string username;

    string command;

    string sender;

    string receiver;


};

#endif // GENERICREQUESTMESSAGE_H
