#include "../Headers/User.h"

void User::setGroup(int group)
{
    User::group = group;
}

void User::setAdress(const sockaddr_in &adress)
{
    User::adress = adress;
}

const sockaddr_in &User::getAdress() const
{
    return adress;
}

int User::getGroup() const
{
    return group;
}
