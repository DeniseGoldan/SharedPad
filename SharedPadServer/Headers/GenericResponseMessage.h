#ifndef SHAREDPADSERVER_GENERICMESSAGE_H
#define SHAREDPADSERVER_GENERICMESSAGE_H

#include <string>

using namespace std;

class GenericResponseMessage {
public:
    GenericResponseMessage();

    virtual ~GenericResponseMessage();

    int getCode() const;

    const string &getCodeDescription() const;

    void setCode(int code);

    void setCodeDescription(const string &codeDescription);


private:
    int code = -1;

    string codeDescription;

    string sender;
public:
    const string &getSender() const;

    void setSender(const string &sender);

    const string &getReceiver() const;

    void setReceiver(const string &receiver);

private:
    string receiver;
};


#endif //SHAREDPADSERVER_GENERICMESSAGE_H
