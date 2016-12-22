#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H
#include <arpa/inet.h>

class ServerConnection
{

public:
    void establishConnection();
private:
    const char * ip = "127.0.0.1";
    in_port_t port = 2024;
    sockaddr_in configuration;

};

#endif // SERVERCONNECTION_H
