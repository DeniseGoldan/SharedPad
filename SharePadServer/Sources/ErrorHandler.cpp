#include "../Headers/ErrorHandler.h"

void ErrorHandler::handleErrorWithoutExit(const char *errorMessage)
{
    printf("%s", errorMessage);
}

void ErrorHandler::exitFailure(const char *errorMessage)
{
    handleErrorWithoutExit(errorMessage);
    exit(EXIT_FAILURE);
}
