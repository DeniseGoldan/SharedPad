#include "../Headers/User.h"

void User::updateLastCheck(const timeval &lastCheck)
{
    User::lastCheck = lastCheck;
}

const timeval &User::getLastCheck() const
{
    return lastCheck;
}

const string &User::getPeerFileContent() const
{
    return peerFileContent;
}

void User::setPeerFileContent(const string &peerFileContent)
{
    User::peerFileContent = peerFileContent;
}

bool User::doesHaveFileContentFromPeer() const
{
    return hasFileContentFromPeer;
}

void User::setHasFileContentFromPeer(bool hasFileContentFromPeer)
{
    User::hasFileContentFromPeer = hasFileContentFromPeer;
}

