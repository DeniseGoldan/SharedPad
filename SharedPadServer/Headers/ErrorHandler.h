#ifndef SHAREPADSERVER_ERRORHANDLER_H
#define SHAREPADSERVER_ERRORHANDLER_H

#include <cstdio>
#include <cstdlib>

class ErrorHandler {
public:

    static void handleErrorWithoutExit(const char *errorMessage);

    static void exitFailure(const char *errorMessage);
};

#endif //SHAREPADSERVER_ERRORHANDLER_H
