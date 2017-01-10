#ifndef NAMESPACESPP_H
#define NAMESPACESPP_H

const int PREFIX_LENGTH = 10;
const int BUFF_SIZE = 256; // 2^8
const int MAX_COMM_NAME_LEN = 40;

const char *const CODE = "code";
const char *const CODE_DESCRIPTION = "code_description";
const char *const COMMAND = "command";
const char *const ARGUMENTS = "arguments";

const char *const CLIENT_IP_ADDRESS = "client_ip_address";
const char *const CLIENT_PORT = "client_port";

const char *const USERNAME = "username";
const char *const SENDER = "sender";
const char *const RECEIVER = "receiver";

const char *const STATUS = "status";
const char *const ACCEPT = "accept";
const char *const DECLINE = "decline";

const char *const UNKNOWN = "unknown";
const int UNKNOWN_CODE = 4000;

const char *const QUERY = "query";
const char *const QUERY_APPROVED = "update_approved";
const int QUERY_APPROVED_CODE = 2000;

const char *const LOGIN = "login";
const char *const LOGIN_APPROVED = "login_approved";
const char *const LOGIN_FAILED = "login_not_approved";
const int LOGIN_APPROVED_CODE = 2001;
const int LOGIN_FAILED_CODE = 4001;

const char *const LOGOUT = "logout";
const char *const LOGOUT_APPROVED = "logout_approved";
const int LOGOUT_APPROVED_CODE = 2002;

const char *const PAIR_REQUEST = "pair_request";
const char *const PAIR_RESPONSE = "pair_response";

const char *const PAIR_INVITATION = "pair_invitation";
const int PAIR_INVITATION_CODE = 20014;

const char *const PAIR_ADDED = "pair_added";
const int PAIR_ADDED_CODE = 20017;

const char *const PAIR_NOT_ADDED = "pair_not_added";
const int PAIR_NOT_ADDED_CODE = 20018;

const char *const INVITATION_SENT = "invitation_sent";
const int INVITATION_SENT_CODE = 20012;

const char *const INVITED_YOURSELF = "invited_yourself";
const int INVITED_YOURSELF_CODE = 40012;

const char *const USER_NOT_LOGGED_IN = "user_not_logged_in";
const int USER_NOT_LOGGED_IN_CODE = 4004;

const char *const ALREADY_PAIRED = "already_paired";
const int ALREADY_PAIRED_CODE = 40013;

const char *const CONNECTION_FAILED = "connection_failed";
const int CONNECTION_FAILED_CODE = 4005;

const char *const JSON_PARSING_FAILED = "json_parsing_failed";
const int JSON_PARSING_FAILED_CODE = 4009;

#endif //NAMESPACESPP_H
