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

    const string &getPeerFileContent() const;

    void setPeerFileContent(const string &peerFileContent);

    bool doesHaveFileContentFromPeer() const;

    void setHasFileContentFromPeer(bool hasFileContentFromPeer);

private:
    sockaddr_in address;
    timeval lastCheck;
    string peerFileContent;
    bool hasFileContentFromPeer;
};

#endif //SHAREDPADSERVER_USER_H
