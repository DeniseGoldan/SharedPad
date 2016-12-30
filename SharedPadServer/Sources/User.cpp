#include "../Headers/User.h"

void User::setGroup(int group)
{
    User::group = group;
}

void User::setAddress(const sockaddr_in &address)
{
    User::address = address;
}

void User::updateCheck(const timeval &lastCheck)
{
    User::lastCheck = lastCheck;
}

int User::getGroup() const
{
    return group;
}

const sockaddr_in &User::getAddress() const
{
    return address;
}

const timeval &User::getLastCheck() const
{
    return lastCheck;
}
