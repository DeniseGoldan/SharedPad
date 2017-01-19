#ifndef CLIENT_HEART_BEAT_SENDER_H
#define CLIENT_HEART_BEAT_SENDER_H

#include<QString>
#include<QMessageBox>

#include <string>
#include <pthread.h>

#include "GenericRequest.h"
#include "GenericResponse.h"
#include "JsonRequestGenerator.h"
#include "StatusCodesAndDescriptions.h"
#include "Client.h"

using namespace std;

class HeartBeatSender
{
public:
    HeartBeatSender();
    QString getUsername() const;
    void setUsername(QString &value);
    void sendUpdates();
    static void *handleUpdating(void *username);

    static GenericRequest *getHeartBeatRequest(string username);

private:
    QString username;

};

#endif // CLIENT_HEART_BEAT_SENDER_H
