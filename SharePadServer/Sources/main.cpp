#include <iostream>
#include <spdlog/spdlog.h>
#include "../Headers/Server.h"

int main()
{
    Server myServer;
    myServer.startListeningSession();
    return 0;
}