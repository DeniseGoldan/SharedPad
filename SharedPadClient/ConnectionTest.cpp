#include "ConnectionTest.h"
auto connTest_logger = spd::stdout_color_mt("connTest_logger");


ConnectionTest::ConnectionTest(){}

void ConnectionTest::sendUpdates()
{
    pthread_t threadId;
    if (0 != pthread_create(&threadId, nullptr, ConnectionTest::handleUpdating, (void*)username.toStdString().c_str()))
    {
        connTest_logger->warn("sendUpdates function has a problem. Thread creation failed.");
    }
}

void *ConnectionTest::handleUpdating(void *username)
{
    //TODO: mutex?
    string usernameAsString = (char*) username;
    GenericRequestMessage updateRequest;
    updateRequest.setCommand(UPDATE_CONN_TEST);
    updateRequest.setUsername(usernameAsString);
    string jsonUpdateRequest = JsonRequestMessageGenerator::getJsonLogRequestMessage(updateRequest);

    while(true){
        //GenericResponseMessage *response = =
        Client::sendRequestToServer(jsonUpdateRequest);
        sleep(3);
    }

    pthread_detach(pthread_self());
    pthread_exit(nullptr);
}


QString ConnectionTest::getUsername() const
{
    return username;
}

void ConnectionTest::setUsername(QString &value)
{
    username = value;
}

