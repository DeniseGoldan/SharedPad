#include "NewsChecker.h"

auto newsChecker_logger = spd::stdout_color_mt("newsChecker_logger");

NewsChecker::NewsChecker(string username){
    this->username = username;
}

NewsChecker::~NewsChecker() {}

void NewsChecker::checkInbox()
{
    GenericRequest *checkNewsRequest = SpecializedRequest::getCheckNewsRequest(username);
    string jsonCheckRequest = JsonRequestGenerator::getJsonLogRequest(*checkNewsRequest);
    while (1)
    {
        GenericResponse *response = Client::sendRequest(jsonCheckRequest);
        switch(response->getCode())
        {
        case HAD_NEWS_CODE:
        {
            QString temp = QString::fromStdString(response->getCodeDescription());
            newsChecker_logger->warn(response->getCode());
            newsChecker_logger->warn(response->getCodeDescription());
            newsChecker_logger->warn(response->getContent());
            emit receiveNewsFromPeer(temp);
            break;
        }
        case PAIR_ADDED_CODE:
        {
            QString temp = QString::fromStdString(response->getCodeDescription());
            newsChecker_logger->warn(response->getCode());
            newsChecker_logger->warn(response->getCodeDescription());
            emit receivePeerUsername(temp);
            break;
        }
        case CONNECTION_FAILED_CODE:
        {
            emit serverCrashed();
        }
        }
        sleep(2);
    }
}
