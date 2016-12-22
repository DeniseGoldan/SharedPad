#ifndef SHAREPADSERVER_NAMESPACESPP_H
#define SHAREPADSERVER_NAMESPACESPP_H

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


const int THREAD_CREATION_FAILURE_CODE = 21;
const char *const READ_FAILURE = "read_failure";
const int READ_FAILURE_CODE = 31;


#endif //SHAREPADSERVER_NAMESPACESPP_H
