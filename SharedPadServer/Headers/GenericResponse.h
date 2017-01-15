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

    void setCode(int code);

    const string &getCodeDescription() const;

    void setCodeDescription(const string &codeDescription);

    const string &getSender() const;

    const string &getReceiver() const;

    const string &getUsername() const;

    const string &getContent() const;

private:
    int code = -1;
    string codeDescription;
    string username;
    string sender;
    string receiver;
    string content;
};


#endif //SERVER_GENERIC_RESPONSE_H
