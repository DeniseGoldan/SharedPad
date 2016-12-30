#ifndef CONNECTIONTEST_H
#define CONNECTIONTEST_H

#include<QString>

#include <string>
#include <pthread.h>

#include "GenericRequestMessage.h"
#include "GenericResponseMessage.h"
#include "JsonRequestMessageGenerator.h"
#include "NamespaceSPP.h"
#include "Client.h"

using namespace std;

class ConnectionTest
{
public:
    ConnectionTest();
    QString getUsername() const;
    void setUsername(QString &value);
    void sendUpdates();
    static void *handleUpdating(void *username);

private:
    QString username;

};

#endif // CONNECTIONTEST_H