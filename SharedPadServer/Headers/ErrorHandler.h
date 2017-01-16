#ifndef SERVER_ERROR_HANDLER_H
#define SERVER_ERROR_HANDLER_H

#include <cstdio>
#include <cstdlib>

class ErrorHandler
{
public:
    static void exitFailure(const char *errorMessage);
};

#endif //SERVER_ERROR_HANDLER_H
