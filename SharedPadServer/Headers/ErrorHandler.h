#ifndef SHAREPADSERVER_ERRORHANDLER_H
#define SHAREPADSERVER_ERRORHANDLER_H


class ErrorHandler {

public:

    static void handleErrorWithoutExit(const char *errorMessage);

    static void handleErrorAndExitFailure(const char *errorMessage);
};


#endif //SHAREPADSERVER_ERRORHANDLER_H
