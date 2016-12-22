//
// Created by denice on 10.12.2016.
//

#include "../Headers/User.h"

const sockaddr_in &User::getAdress() const
{
    return adress;
}

int User::getGroup() const
{
    return group;
}

void User::setGroup(int group)
{
    User::group = group;
}

void User::setAdress(const sockaddr_in &adress)
{
    User::adress = adress;
}
