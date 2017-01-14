#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <string.h>

#include "GenericRequestMessage.h"
#include "GenericResponseMessage.h"
#include "JsonRequestMessageGenerator.h"
#include "Client.h"

#include "spdlog/spdlog.h"

class Worker : public QObject {
    Q_OBJECT

public:
    Worker(string username);
    ~Worker();

public slots:
    void checkInbox();

signals:
    void receiveFileFromPartner(QString content);

private:
    string username;
};

#endif // WORKER_H

