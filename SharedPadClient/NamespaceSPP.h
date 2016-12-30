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
const int UNKNOWN_CODE = 41;

const char *const LOGIN = "login";
const char *const LOGIN_APPROVED = "login_approved";
const char *const LOGIN_FAILED = "login_not_approved";
const int LOGIN_APPROVED_CODE = 11;
const int LOGIN_FAILED_CODE = 91;

const char *const LOGOUT = "logout";
const char *const LOGOUT_APPROVED = "logout_approved";
const char *const LOGOUT_FAILED = "logout_not_approved";
const int LOGOUT_APPROVED_CODE = 121;
const int LOGOUT_FAILED_CODE = 921;

const char *const UPDATE_CONN_TEST = "update_conn_test";
const char *const UPDATE_CONN_TEST_APPROVED = "update_conn_test_approved";
const int UPDATE_CONN_TEST_APPROVED_CODE = 911;

const char *const USER_NOT_LOGGED_IN = "user_not_logged_in";
const int USER_NOT_LOGGED_IN_CODE = 333;

const char *const CONNECTION_FAILED = "connection_failed";
const int CONNECTION_FAILED_CODE = 81;

const int THREAD_CREATION_FAILED_CODE = 21;

const char *const READ_FAILED = "reading_failed";
const int READ_FAILED_CODE = 31;

const char *const WRITE_FAILED = "writing_failed";
const int WRITE_FAILED_CODE = 101;

const char *const JSON_PARSING_FAILED = "json_parsing_failed";
const int JSON_PARSING_FAILED_CODE = 401;


#endif //NAMESPACESPP_H
