#ifndef SHAREPADSERVER_ERRORHANDLER_H
#define SHAREPADSERVER_ERRORHANDLER_H

#include <cstdio>
#include <cstdlib>

class ErrorHandler {

public:
    static void handleErrorAndExitFailure(const char *errorMessage);

    static void handleErrorWithoutExit(const char *string);
};


#endif //SHAREPADSERVER_ERRORHANDLER_H
