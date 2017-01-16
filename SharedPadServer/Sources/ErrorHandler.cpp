#include "../Headers/ErrorHandler.h"

void ErrorHandler::exitFailure(const char *errorMessage)
{
    printf("%s", errorMessage);
    exit(EXIT_FAILURE);
}
