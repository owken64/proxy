#pragma once

#include <arpa/inet.h> // struct sockaddr_in, struct sockaddr, inet_ntoa()
#include <sys/select.h>

#include "Config.h"
#include "Bool.h"

// From Client
extern int sockServer;
extern struct sockaddr_in sockAddrServer;
extern int sockClient[MAX_CONNECTION];
extern struct sockaddr_in sockAddrClient[MAX_CONNECTION];
extern BOOL availableSockClient[MAX_CONNECTION];

// To Server
extern int sockGamesv;
extern int sockMsgsv;
extern int sockLoginsv;

extern struct sockaddr_in sockAddrGamesv;
extern struct sockaddr_in sockAddrMsgsv;
extern struct sockaddr_in sockAddrLoginsv;

// select()用
extern fd_set readfds;

// Message
extern char messageRecvClient[MAX_CONNECTION][MSG_LENGTH];
extern BOOL flgMsgRecvClient[MAX_CONNECTION];
extern char messageRecvGamesv[MSG_LENGTH];
extern BOOL flgMsgRecvGamesv;
extern char messageRecvMsgsv[MSG_LENGTH];
extern BOOL flgMsgRecvMsgsv;
extern char messageRecvLoginsv[MSG_LENGTH];
extern BOOL flgMsgRecvLoginsv;

extern char messageSendClient[MAX_CONNECTION][MSG_LENGTH];
extern BOOL flgMsgSendClient[MAX_CONNECTION];
extern char messageSendGamesv[MSG_LENGTH];
extern BOOL flgMsgSendGamesv;
extern char messageSendMsgsv[MSG_LENGTH];
extern BOOL flgMsgSendMsgsv;
extern char messageSendLoginsv[MSG_LENGTH];
extern BOOL flgMsgSendLoginsv;
