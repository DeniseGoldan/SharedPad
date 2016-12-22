#include <string>

#ifndef SHAREPADSERVER_GENERICMESSAGE_H
#define SHAREPADSERVER_GENERICMESSAGE_H


class GenericResponseMessage {
public:
    GenericResponseMessage();

    virtual ~GenericResponseMessage();

    int getCode() const;

    const std::string &getCodeDescription() const;

    void setCode(int code);

    void setCodeDescription(const std::string &codeDescription);

private:
    int code = -1;

    std::string codeDescription;
};


#endif //SHAREPADSERVER_GENERICMESSAGE_H
