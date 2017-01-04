#ifndef SHAREDPADSERVER_ERRORHANDLER_H
#define SHAREDPADSERVER_ERRORHANDLER_H

#include <cstdio>
#include <cstdlib>

class ErrorHandler {
public:
    static void handleErrorWithoutExit(const char *errorMessage);

    static void exitFailure(const char *errorMessage);
};

#endif //SHAREDPADSERVER_ERRORHANDLER_H
