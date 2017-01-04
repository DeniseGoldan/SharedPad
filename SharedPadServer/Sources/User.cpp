#include "../Headers/User.h"

void User::setAddress(const sockaddr_in &address)
{
    User::address = address;
}

void User::updateLastCheck(const timeval &lastCheck)
{
    User::lastCheck = lastCheck;
}

const sockaddr_in &User::getAddress() const
{
    return address;
}

const timeval &User::getLastCheck() const
{
    return lastCheck;
}

