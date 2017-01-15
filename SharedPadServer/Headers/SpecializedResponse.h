#ifndef SERVER_SPECIALIZED_RESPONSE_H
#define SERVER_SPECIALIZED_RESPONSE_H

#include "GenericResponse.h"

class SpecializedResponse
{
public:
    static GenericResponse *getLoginFailedResponse();

    static GenericResponse *getLoginApprovedResponse();

    static GenericResponse *getLogoutApprovedResponse();

    static GenericResponse *getUnknownResponse();

    static GenericResponse *getUserNotLoggedInResponse();

    static GenericResponse *getHeartBeatApprovedResponse();

    static GenericResponse *getPairAddedResponse();

    static GenericResponse *getAlreadyPairedResponse();

    static GenericResponse *getInvitedYourselfResponse();

    static GenericResponse *getYourAreSingleResponse();

    static GenericResponse *getSentNewsToPeerResponse();
};


#endif //SERVER_SPECIALIZED_RESPONSE_H
