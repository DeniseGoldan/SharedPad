#include "PairSyncWorker.h"

auto pairSyncWorker_logger = spd::stdout_color_mt("pairSyncWorker_logger");

PairSyncWorker::PairSyncWorker(string username){
    this->username = username;
}

PairSyncWorker::~PairSyncWorker() {}

void PairSyncWorker::checkInbox()
{
    GenericRequestMessage checkRequest;
    checkRequest.setCommand(CHECK_NEWS);
    checkRequest.setUsername(username);
    string jsonCheckRequest = JsonRequestMessageGenerator::getJsonLogRequestMessage(checkRequest);

    while (1)
    {
        GenericResponseMessage *response = Client::sendRequestToServer(jsonCheckRequest);
        switch(response->getCode())
        {
            case HAD_NEWS_CODE:
            {
                QString temp = QString::fromStdString(response->getCodeDescription());
                worker_logger->warn(response->getCode());
                worker_logger->warn(response->getCodeDescription());
                worker_logger->warn(response->getContent());
                emit receiveFileFromPartner(temp);
                break;
            }
            case NOTHING_NEW_CODE:
            {
                worker_logger->warn(response->getContent());
                break;
            }
        }
        sleep(3);
    }
}


QString peerUsername = ui->peerUsernameTag->text();
sync_logger->warn(peerUsername.toStdString());
if (peerUsername.isNull())
{
    QMessageBox::information(this, tr("Error message"), "You must have a peer. Check if you have a peer or send a request!");
    return;
}

Client * client = new Client();
GenericResponseMessage * responseFromServer = client->synchronize(username.toStdString(), ui->textEdit->toPlainText().toStdString());

switch(responseFromServer->getCode())
{
    case YOU_ARE_SINGLE_CODE :
    {
        QMessageBox::information(this,"Error message","You do not have a pair.");
        break;
    }
    case SENT_NEWS_TO_PEER_CODE:
    {
        QMessageBox::information(this,"Information","You and you peer now share the same text content.");
        break;
    }
}
