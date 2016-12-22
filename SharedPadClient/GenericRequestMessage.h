#ifndef GENERICREQUESTMESSAGE_H
#define GENERICREQUESTMESSAGE_H

#include <string>

class GenericRequestMessage
{
public:
    GenericRequestMessage();

    ~GenericRequestMessage();

    void setUsername(const std::string &username);

    void setCommand(const std::string &command);

    const std::string &getUsername() const;

    const std::string &getCommand() const;

private:

    std::string username;

    std::string command;

};

#endif // GENERICREQUESTMESSAGE_H
