#ifndef JSONMESSAGEGENERATOR_H
#define JSONMESSAGEGENERATOR_H

#include <string>
#include "rapidjson/writer.h"
#include "GenericRequestMessage.h"

class JsonRequestMessageGenerator {
public:
    JsonRequestMessageGenerator();
    static std::string getJsonLoginRequestMessage(const GenericRequestMessage &message);
private:
};

#endif // JSONMESSAGEGENERATOR_H


