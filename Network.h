#pragma once
#include <sys/socket.h>

#define NETWORK_CONNECT_SUCCESS 0
#define NETWORK_CONNECT_FAILURE -1

int NetworkInit();
void NetworkClose();

