#include "HeartBeatSender.h"
auto handleUpdating_logger = spd::stdout_color_mt("handleUpdating_logger");

HeartBeatSender::HeartBeatSender(){}

void HeartBeatSender::sendUpdates()
{
    string castedArgument = username.toStdString();
    char * argument = (char*) malloc ((1+castedArgument.length())*sizeof(char));
    strcpy(argument, castedArgument.c_str());

    pthread_t threadId;
    if (0 != pthread_create(&threadId, nullptr, HeartBeatSender::handleUpdating, (void*)argument))
    {
        handleUpdating_logger->warn("sendUpdates function has a problem. Thread creation failed.");
    }
}

void *HeartBeatSender::handleUpdating(void *argument)
{
    char *castedArgument = (char *)argument;
    string username(castedArgument);
    handleUpdating_logger->info(castedArgument);

    GenericRequest * updateRequest = getHeartBeatRequest(username);
    string jsonUpdateRequest = JsonRequestGenerator::getJsonLogRequest(*updateRequest);

    while(true)
    {
        GenericResponse* response = Client::sendRequest(jsonUpdateRequest);
        if (response->getCode() == CONNECTION_FAILED_CODE)
        {
            handleUpdating_logger->critical("SERVER CRASHED");
            exit(EXIT_FAILURE);
        }
        sleep(5);
    }
}

GenericRequest *HeartBeatSender::getHeartBeatRequest(string username)
{
    GenericRequest *updateRequest = new GenericRequest;
    updateRequest->setCommand(HEARTBEAT);
    updateRequest->setUsername(username);
    return updateRequest;
}

QString HeartBeatSender::getUsername() const
{
    return username;
}

void HeartBeatSender::setUsername(QString &value)
{
    username = value;
}

