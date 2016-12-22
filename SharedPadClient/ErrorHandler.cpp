#include <cstdio>
#include <cstdlib>
#include "ErrorHandler.h"

void ErrorHandler::handleErrorAndExitFailure(const char *errorMessage)
{
    handleErrorWithoutExit(errorMessage);
    exit(EXIT_FAILURE);
}

void ErrorHandler::handleErrorWithoutExit(const char *errorMessage)
{
    printf("%s", errorMessage);
}
