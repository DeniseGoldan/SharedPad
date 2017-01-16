#include "../Headers/SpecializedResponse.h"
#include "../Headers/StatusCodesAndDescriptions.h"

GenericResponse *SpecializedResponse::getUnknownResponse()
{
    GenericResponse *response = new GenericResponse();
    response->setCode(UNKNOWN_CODE);
    response->setCodeDescription(UNKNOWN);
    return response;
}

GenericResponse *SpecializedResponse::getLoginApprovedResponse()
{
    GenericResponse *response = new GenericResponse();
    response->setCode(LOGIN_APPROVED_CODE);
    response->setCodeDescription(LOGIN_APPROVED);
    return response;
}

GenericResponse *SpecializedResponse::getLoginFailedResponse()
{
    GenericResponse *response = new GenericResponse();
    response->setCode(LOGIN_FAILED_CODE);
    response->setCodeDescription(LOGIN_FAILED);
    return response;
}

GenericResponse *SpecializedResponse::getLogoutApprovedResponse()
{
    GenericResponse *response = new GenericResponse();
    response->setCode(LOGOUT_APPROVED_CODE);
    response->setCodeDescription(LOGOUT_APPROVED);
    return response;
}

GenericResponse *SpecializedResponse::getHeartBeatApprovedResponse()
{
    GenericResponse *response = new GenericResponse();
    response->setCode(HEARTBEAT_APPROVED_CODE);
    response->setCodeDescription(HEARTBEAT_APPROVED);
    return response;
}

GenericResponse *SpecializedResponse::getUserNotLoggedInResponse()
{
    GenericResponse *response = new GenericResponse();
    response->setCode(USER_NOT_LOGGED_IN_CODE);
    response->setCodeDescription(USER_NOT_LOGGED_IN);
    return response;
}

GenericResponse *SpecializedResponse::getInvitedYourselfResponse()
{
    GenericResponse *response = new GenericResponse();
    response->setCode(INVITED_YOURSELF_CODE);
    response->setCodeDescription(INVITED_YOURSELF);
    return response;
}

GenericResponse *SpecializedResponse::getAlreadyPairedResponse()
{
    GenericResponse *response = new GenericResponse();
    response->setCode(ALREADY_PAIRED_CODE);
    response->setCodeDescription(ALREADY_PAIRED);
    return response;
}

GenericResponse *SpecializedResponse::getPairAddedResponse()
{
    GenericResponse *response = new GenericResponse();
    response->setCode(PAIR_ADDED_CODE);
    response->setCodeDescription(PAIR_ADDED);
    return response;
}

GenericResponse *SpecializedResponse::getYourAreSingleResponse()
{
    GenericResponse *response = new GenericResponse();
    response->setCode(YOU_ARE_SINGLE_CODE);
    response->setCodeDescription(YOU_ARE_SINGLE);
    return response;
}

GenericResponse *SpecializedResponse::getSentNewsToPeerResponse()
{
    GenericResponse *response = new GenericResponse();
    response->setCode(SENT_NEWS_TO_PEER_CODE);
    response->setCodeDescription(SENT_NEWS_TO_PEER);
    return response;
}
