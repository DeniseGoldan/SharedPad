#ifndef SHAREPADSERVER_USER_H
#define SHAREPADSERVER_USER_H

#include <netinet/in.h>
#include <sys/time.h>

class User {
public:

    const sockaddr_in &getAddress() const;

    int getGroup() const;

    void setGroup(int group);

    void setAddress(const sockaddr_in &address);

private:
    int group;
    sockaddr_in address;
    timeval lastCheck;
public:
    void updateCheck(const timeval &lastCheck);

public:
    const timeval &getLastCheck() const;

};

#endif //SHAREPADSERVER_USER_H
