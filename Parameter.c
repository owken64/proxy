#include "Parameter.h"
#include <arpa/inet.h> // struct sockaddr_in, struct sockaddr, inet_ntoa()
#include <sys/select.h>

#include "Config.h"
#include "Bool.h"

// From Client
int sockServer;
struct sockaddr_in sockAddrServer;
int sockClient[MAX_CONNECTION];
struct sockaddr_in sockAddrClient[MAX_CONNECTION];
char availableSockClient[MAX_CONNECTION];

// To Server
int sockGamesv;
int sockMsgsv;
int sockLoginsv;

struct sockaddr_in sockAddrGamesv;
struct sockaddr_in sockAddrMsgsv;
struct sockaddr_in sockAddrLoginsv;

// select()用
fd_set readfds;

// Message
char messageRecvClient[MAX_CONNECTION][MSG_LENGTH];
BOOL flgMsgRecvClient[MAX_CONNECTION];
char messageRecvGamesv[MSG_LENGTH];
BOOL flgMsgRecvGamesv;
char messageRecvMsgsv[MSG_LENGTH];
BOOL flgMsgRecvMsgsv;
char messageRecvLoginsv[MSG_LENGTH];
BOOL flgMsgRecvLoginsv;

char messageSendClient[MAX_CONNECTION][MSG_LENGTH];
BOOL flgMsgSendClient[MAX_CONNECTION];
char messageSendGamesv[MSG_LENGTH];
BOOL flgMsgSendGamesv;
char messageSendMsgsv[MSG_LENGTH];
BOOL flgMsgSendMsgsv;
char messageSendLoginsv[MSG_LENGTH];
BOOL flgMsgSendLoginsv;

