#include <cstdio>
#include <cstdlib>
#include "../Headers/ErrorHandler.h"

void ErrorHandler::handleErrorWithoutExit(const char *errorMessage)
{
    printf("%s", errorMessage);
}

void ErrorHandler::handleErrorAndExitFailure(const char *errorMessage)
{
    handleErrorWithoutExit(errorMessage);
    exit(EXIT_FAILURE);
}
