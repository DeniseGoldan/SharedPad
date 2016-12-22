#ifndef SHAREPADSERVER_MESSAGECONTENT_H
#define SHAREPADSERVER_MESSAGECONTENT_H

#include "ServerFrame.h"

class MessageContent {
public:
    MessageContent();

    ~MessageContent();

    MessageContent &setCode(int desiredStatusCode);

    MessageContent &setCodeDescription(const char *desiredDescription);

    MessageContent &setFrame(const Frame *desiredFrame);

    int getCode() const;

    const char *getCodeDescription()const;

    Frame *getFrame()const;

private:
    int code = -1;
    char *codeDescription = nullptr;
    Frame *frame = nullptr;
};


#endif //SHAREPADSERVER_MESSAGECONTENT_H
