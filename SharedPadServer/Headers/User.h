#ifndef SHAREPADSERVER_USER_H
#define SHAREPADSERVER_USER_H

#include <netinet/in.h>

class User {
public:

    const sockaddr_in &getAdress() const;

    int getGroup() const;

    void setGroup(int group);

    void setAdress(const sockaddr_in &adress);

private:
    int group;
    sockaddr_in adress;


};

#endif //SHAREPADSERVER_USER_H
