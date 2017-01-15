#include "QuerySender.h"
auto handleUpdating_logger = spd::stdout_color_mt("handleUpdating_logger");

QuerySender::QuerySender(){}

void QuerySender::sendUpdates()
{
    string castedArgument = username.toStdString();
    char * argument = (char*) malloc ((1+castedArgument.length())*sizeof(char));
    strcpy(argument, castedArgument.c_str());

    pthread_t threadId;
    if (0 != pthread_create(&threadId, nullptr, QuerySender::handleUpdating, (void*)argument))
    {
        handleUpdating_logger->warn("sendUpdates function has a problem. Thread creation failed.");
    }
}

void *QuerySender::handleUpdating(void *argument)
{
    char *castedArgument = (char *)argument;
    string username(castedArgument);

    handleUpdating_logger->info(castedArgument);

    GenericRequestMessage updateRequest;
    updateRequest.setCommand(HEARTBEAT);
    updateRequest.setUsername(username);

    string jsonUpdateRequest = JsonRequestMessageGenerator::getJsonLogRequestMessage(updateRequest);

    while(true)
    {
        GenericResponseMessage* response = Client::sendRequestToServer(jsonUpdateRequest);
        if (response->getCode() == CONNECTION_FAILED_CODE)
        {
            handleUpdating_logger->critical("-----SERVER CRASHED-----");
            exit(EXIT_FAILURE);
        }
        sleep(5);
    }
}

QString QuerySender::getUsername() const
{
    return username;
}

void QuerySender::setUsername(QString &value)
{
    username = value;
}

