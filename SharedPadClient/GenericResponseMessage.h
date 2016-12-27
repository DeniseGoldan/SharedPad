#ifndef GENERICRESPONSEMESSAGE_H
#define GENERICRESPONSEMESSAGE_H

#include <string>

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


#endif //GENERICRESPONSEMESSAGE_H
