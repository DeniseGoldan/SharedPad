#include <string>
#include "GenericResponseMessage.h"

#ifndef SHAREPADSERVER_LOGINMESSAGE_H
#define SHAREPADSERVER_LOGINMESSAGE_H


class LoginResponseMessage : public GenericResponseMessage {
public:
    LoginResponseMessage();

    ~LoginResponseMessage();

    const std::string &getUsername() const;

    const std::string &getIpAddress() const;

    int getPort() const;

    void setUsername(const std::string &username);

    void setIpAddress(const std::string &ipAddress);

    void setPort(int port);

private:
    std::string username;
    std::string ipAddress;
    int port = -1;

};


#endif //SHAREPADSERVER_LOGINMESSAGE_H
