#ifndef NAMESPACESPP_H
#define NAMESPACESPP_H

const int PREFIX_LENGTH = 10;
const int BUFF_SIZE = 256; // 2^8
const int MAX_COMM_NAME_LEN = 40;
//CONNECTION
const char *const CONNECTION_FAILED = "connection_failed";
const int CONNECTION_FAILED_CODE = 400;
//PARSING
const char *const JSON_PARSING_FAILED = "json_parsing_failed";
const int JSON_PARSING_FAILED_CODE = 401;
//WRITING
const char *const WRITE_FAILED = "write_failed";
const int WRITE_FAILED_CODE = 402;
// client related
const char *const CLIENT_IP_ADDRESS = "client_ip_address";
const char *const CLIENT_PORT = "client_port";
// Response and Request specific fields
const char *const COMMAND = "command";
const char *const ARGUMENTS = "arguments";
const char *const CODE = "code";
const char *const CODE_DESCRIPTION = "code_description";
const char *const USERNAME = "username";
const char *const SENDER = "sender";
const char *const RECEIVER = "receiver";
const char *const CONTENT = "content";
//UNKNOWN - basic message
const char *const UNKNOWN = "unknown";
const int UNKNOWN_CODE = 403;
//LOGIN
const char *const LOGIN = "login";
const char *const LOGIN_APPROVED = "login_approved";
const int LOGIN_APPROVED_CODE = 200;
const char *const LOGIN_FAILED = "login_failed";
const int LOGIN_FAILED_CODE = 404;
//LOGOUT
const char *const LOGOUT = "logout";
const char *const LOGOUT_APPROVED = "logout_approved";
const int LOGOUT_APPROVED_CODE = 201;
//HEARTBEAT
const char *const HEARTBEAT = "heartbeat";
const char *const HEARTBEAT_APPROVED = "heartbeat_approved";
const int HEARTBEAT_APPROVED_CODE = 202;
//PAIR and UNPAIR
const char *const PAIR = "pair_request";
const char *const UNPAIR = "unpair_request";
const char *const PAIR_ADDED = "pair_added";
const int PAIR_ADDED_CODE = 203;
const char *const INVITED_YOURSELF = "invited_yourself";
const int INVITED_YOURSELF_CODE = 405;
const char *const USER_NOT_LOGGED_IN = "user_not_logged_in";
const int USER_NOT_LOGGED_IN_CODE = 406;
const char *const ALREADY_PAIRED = "already_paired";
const int ALREADY_PAIRED_CODE = 407;
//SEND_NEWS
const char *const SEND_NEWS = "send_news_to_peer";
const char *const YOU_ARE_SINGLE = "you_are_single";
const int YOU_ARE_SINGLE_CODE = 408;
const char *const SENT_NEWS_TO_PEER = "sent_news_to_peer";
const int SENT_NEWS_TO_PEER_CODE = 204;
//CHECK NEWS
const char *const CHECK_NEWS = "check_news";
const char *const HAD_NEWS = "had_news";
const int HAD_NEWS_CODE = 205;
const char * const NOTHING_NEW = "nothing_new";
const int NOTHING_NEW_CODE = 206;

#endif //NAMESPACESPP_H
