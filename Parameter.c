#include "Parameter.h"
#include <arpa/inet.h> // struct sockaddr_in, struct sockaddr, inet_ntoa()

#include "Config.h"

// From Client
int sockServer;
struct sockaddr_in sockAddrServer;
int sockClient[MAX_CONNECTION];
struct sockaddr_in sockAddrClient[MAX_CONNECTION];
bool availableSockClient[MAX_CONNECTION];
int nConnection; // 現在の接続数

// To Server
int sockGamesv;
int sockMsgsv;
int sockLoginsv;

struct sockaddr_in sockAddrGamesv;
struct sockaddr_in sockAddrMsgsv;
struct sockaddr_in sockAddrLoginsv;