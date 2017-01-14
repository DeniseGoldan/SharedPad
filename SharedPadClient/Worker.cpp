#include "Worker.h"

auto worker_logger = spd::stdout_color_mt("worker_logger");

Worker::Worker(string username){
    this->username = username;
}

Worker::~Worker() {}

void Worker::checkInbox()
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
        sleep(2);
    }
}
