#ifndef SERVER_GENERIC_RESPONSE_H
#define SERVER_GENERIC_RESPONSE_H

#include <string>

using namespace std;

class GenericResponse
{
public:
    GenericResponse();

    ~GenericResponse();

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

    const string &getContent() const;

    void setContent(const string &content);

private:
    int code = -1;
    string codeDescription;
    string username;
    string sender;
    string receiver;
    string content;
};


#endif //SERVER_GENERIC_RESPONSE_H
