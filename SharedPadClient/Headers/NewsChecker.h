#ifndef CLIENT_NEWS_CHECKER_H
#define CLIENT_NEWS_CHECKER_H

#include <QObject>
#include <string.h>

#include "GenericRequest.h"
#include "GenericResponse.h"
#include "JsonRequestGenerator.h"
#include "Client.h"

#include "spdlog/spdlog.h"

class NewsChecker : public QObject {
    Q_OBJECT

public:
    NewsChecker(string username);
    ~NewsChecker();

public slots:
    void checkInbox();

signals:
    void receiveNewsFromPeer(QString news);
    void receivePeerUsername(QString peerUsername);
    void serverCrashed();

private:
    string username;
};

#endif // CLIENT_NEWS_CHECKER_H

