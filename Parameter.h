#pragma once

#include <arpa/inet.h> // struct sockaddr_in, struct sockaddr, inet_ntoa()

#include "Config.h"

// From Client
extern int sockServer;
extern struct sockaddr_in sockAddrServer;
extern int sockClient[MAX_CONNECTION];
extern struct sockaddr_in sockAddrClient[MAX_CONNECTION];
extern bool availableSockClient[MAX_CONNECTION];
extern int nConnection; // 現在の接続数

// To Server
extern int sockGamesv;
extern int sockMsgsv;
extern int sockLoginsv;

extern struct sockaddr_in sockAddrGamesv;
extern struct sockaddr_in sockAddrMsgsv;
extern struct sockaddr_in sockAddrLoginsv;
