#include <cstdlib>
#include <cstring>
#include "ServerMessageContent.h"
#include "ServerFrame.h"

MessageContent::MessageContent() {
}

MessageContent::~MessageContent() {
    free(codeDescription);
    delete (frame);
}

MessageContent &MessageContent::setCode(int desiredStatusCode) {
    code = desiredStatusCode;
    return *this;
}

MessageContent &MessageContent::setCodeDescription(const char *desiredDescription) {
    codeDescription = (char *) malloc(sizeof(char) * (1 + strlen(desiredDescription)));
    strcpy(codeDescription, desiredDescription);
    return *this;
}

MessageContent &MessageContent::setFrame(const Frame *desiredFrame) {
    if (desiredFrame == nullptr) {
        frame = nullptr;
        delete (desiredFrame);
    } else {
        Frame *newFrame = new Frame(*desiredFrame);
        frame = newFrame;
    }
    return *this;
}

int MessageContent::getCode() const {
    return code;
}

const char *MessageContent::getCodeDescription() const {
    return codeDescription;
}

Frame *MessageContent::getFrame() const {
    return frame;
}
