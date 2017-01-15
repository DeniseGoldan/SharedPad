#ifndef SERVER_JSON_RESPONSE_GENERATOR_H
#define SERVER_JSON_RESPONSE_GENERATOR_H

#include <string>

#include "../include/rapidjson/writer.h"

#include "StatusCodesAndDescriptions.h"
#include "GenericResponse.h"

using namespace rapidjson;
using namespace std;

class JsonResponseGenerator
{
public:
    static string getJsonResponse(const GenericResponse &response);

private:
    JsonResponseGenerator();
};

#endif //SERVER_JSON_RESPONSE_GENERATOR_H
