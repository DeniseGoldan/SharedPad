#ifndef QUERYSENDER_H
#define QUERYSENDER_H

#include<QString>

#include <string>
#include <pthread.h>

#include "GenericRequestMessage.h"
#include "GenericResponseMessage.h"
#include "JsonRequestMessageGenerator.h"
#include "StatusCodesAndDescriptions.h"
#include "Client.h"

using namespace std;

class QuerySender
{
public:
    QuerySender();
    QString getUsername() const;
    void setUsername(QString &value);
    void sendUpdates();
    static void *handleUpdating(void *username);

private:
    QString username;

};

#endif // QUERYSENDER_H
