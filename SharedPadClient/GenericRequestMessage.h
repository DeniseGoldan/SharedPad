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

    const string &getIp() const;

    void setIp(const string &ip);

    int getPort() const;

    void setPort(int port);

    int getLine() const;

    void setLine(int line);

    const string &getContent() const;

    void setContent(const string &content);


private:

    string username;

    string command;

    string sender;

    string receiver;

    string ip;

    int port  = -1;

    int line = -1;

    string content;

};

#endif  // GENERICREQUESTMESSAGE_H
