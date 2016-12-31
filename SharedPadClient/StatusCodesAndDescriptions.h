#ifndef NAMESPACESPP_H
#define NAMESPACESPP_H

const int PREFIX_LENGTH = 10;
const int BUFF_SIZE = 256; // 2^8

const char *const CODE = "code";
const char *const CODE_DESCRIPTION = "code_description";
const char *const COMMAND = "command";
const char *const ARGUMENTS = "arguments";

const char *const CLIENT_IP_ADDRESS = "client_ip_address";
const char *const CLIENT_PORT = "client_port";

const char *const USERNAME = "username";

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
const char *const LOGOUT_FAILED = "logout_not_approved";
const int LOGOUT_APPROVED_CODE = 2002;
const int LOGOUT_FAILED_CODE = 4002;

const char *const USER_NOT_LOGGED_IN = "user_not_logged_in";
const int USER_NOT_LOGGED_IN_CODE = 4004;

const char *const CONNECTION_FAILED = "connection_failed";
const int CONNECTION_FAILED_CODE = 4005;

const int THREAD_CREATION_FAILED_CODE = 4006;

const char *const READ_FAILED = "reading_failed";
const int READ_FAILED_CODE = 4007;

const char *const WRITE_FAILED = "writing_failed";
const int WRITE_FAILED_CODE = 4008;

const char *const JSON_PARSING_FAILED = "json_parsing_failed";
const int JSON_PARSING_FAILED_CODE = 4009;


#endif //NAMESPACESPP_H
