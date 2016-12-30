#ifndef GENERICREQUESTMESSAGE_H
#define GENERICREQUESTMESSAGE_H

#include <string>

using namespace std;

class GenericRequestMessage
{
public:
    GenericRequestMessage();

    ~GenericRequestMessage();

    void setUsername(const string &username);

    void setCommand(const string &command);

    const string &getUsername() const;

    const string &getCommand() const;

private:

    string username;

    string command;

};

#endif // GENERICREQUESTMESSAGE_H
