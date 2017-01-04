#ifndef SHAREDPADSERVER_USER_H
#define SHAREDPADSERVER_USER_H

#include <netinet/in.h>
#include <sys/time.h>

#include <string>

using namespace std;

class User {
public:

    void setAddress(const sockaddr_in &address);

    void updateLastCheck(const timeval &lastCheck);

    const sockaddr_in &getAddress() const;

    const timeval &getLastCheck() const;

private:
    sockaddr_in address;
    timeval lastCheck;

};

#endif //SHAREDPADSERVER_USER_H
