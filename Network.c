#include "Network.h"
#include "Parameter.h"
#include "Const.h"

#include <stdio.h> //printf(), fprintf(), perror()
#include <sys/socket.h> //socket(), bind(), accept(), listen()
#include <arpa/inet.h> // struct sockaddr_in, struct sockaddr, inet_ntoa()
#include <stdlib.h> //atoi(), exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <string.h> //memset()
#include <unistd.h> //close()

#define QUEUELIMIT 5

int ConnectClient();
int ConnectGamesv();
int ConnectMsgsv();
int ConnectLoginsv();
void CloseClient();
void CloseGamesv();
void ConnectMsgsv();
void ConnectLoginsv();

int NetworkInit(){
	
	// パラメータの初期化
	nConnection = 0;
	for (int i = 0; i < MAX_CONNECTION; i++){
	  availableSockClient[i] = FALSE;
	}
	
	// サーバーへの接続
	if (ConnectGamesv() == NETWORK_CONNECT_FAILURE) {
		return NETWORK_CONNECT_FAILURE;
	}
	
	if (ConnectMsgsv() == NETWORK_CONNECT_FAILURE) {
		return NETWORK_CONNECT_FAILURE;
	}
	
	if (ConnectLoginsv() == NETWORK_CONNECT_FAILURE){
		return NETWORK_CONNECT_FAILURE;
	}

	// クライアントとの接続準備
	if (ConnectClient() == NETWORK_CONNECT_FAILURE) {
		return NETWORK_CONNECT_FAILURE;
	}

	return NETWORK_CONNECT_SUCCESS;
}

void NetworkClose(){
	CloseClient();
	CloseGamesv();
	ConnectMsgsv();
	ConnectLoginsv();
}

int ConnectClient(){

    if ((sockServer = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ){
        perror("socket() failed.");
        return NETWORK_CONNECT_FAILURE;
    }

    memset(&sockAddrServer, 0, sizeof(sockAddrServer));
    sockAddrServer.sin_family      = AF_INET;
    sockAddrServer.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddrServer.sin_port        = htons(PORT_CLIENT);

    if (bind(sockServer, (struct sockaddr *) &sockAddrServer, sizeof(sockAddrServer) ) < 0 ) {
        perror("bind() failed.");
        return NETWORK_CONNECT_FAILURE;
    }

    if (listen(sockServer, QUEUELIMIT) < 0) {
        perror("listen() failed.");
		return NETWORK_CONNECT_FAILURE;
    }

	return NETWORK_CONNECT_SUCCESS;
}

int ConnectGamesv(){
	// IPv4 TCP のソケットを作成する
    if((sockGamesv = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return NETWORK_CONNECT_FAILURE;
    }
 
    // 送信先アドレスとポート番号を設定する
    sockAddrGamesv.sin_family = AF_INET;
    sockAddrGamesv.sin_port = htons(PORT_GAMESV);
    sockAddrGamesv.sin_addr.s_addr = inet_addr(IP_GAMESV);
 
    // サーバ接続（TCP の場合は、接続を確立する必要がある）
    connect(sockGamesv, (struct sockaddr *)&sockAddrGamesv, sizeof(struct sockaddr_in));
 
	return NETWORK_CONNECT_SUCCESS;
}

int ConnectMsgsv(){
	// IPv4 TCP のソケットを作成する
    if((sockMsgsv = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return NETWORK_CONNECT_FAILURE;
    }
 
    // 送信先アドレスとポート番号を設定する
    sockAddrMsgsv.sin_family = AF_INET;
    sockAddrMsgsv.sin_port = htons(PORT_MSGSV);
    sockAddrMsgsv.sin_addr.s_addr = inet_addr(IP_MSGSV);
 
    // サーバ接続（TCP の場合は、接続を確立する必要がある）
    connect(sockMsgsv, (struct sockaddr *)&sockAddrMsgsv, sizeof(struct sockaddr_in));

	return NETWORK_CONNECT_SUCCESS;
}

int ConnectLoginsv(){
	// IPv4 TCP のソケットを作成する
    if((sockLoginsv = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return NETWORK_CONNECT_FAILURE;
    }
 
    // 送信先アドレスとポート番号を設定する
    sockAddrLoginsv.sin_family = AF_INET;
    sockAddrLoginsv.sin_port = htons(PORT_MSGSV);
    sockAddrLoginsv.sin_addr.s_addr = inet_addr(IP_MSGSV);
 
    // サーバ接続（TCP の場合は、接続を確立する必要がある）
    connect(sockLoginsv, (struct sockaddr *)&sockAddrLoginsv, sizeof(struct sockaddr_in));

	return NETWORK_CONNECT_SUCCESS;
}

void CloseClient(){
	for (int i = 0; i < MAX_CONNECTION; i++){
		if (availableSockClient[i]) {
			close(sockClient);
			availableSockClient[i] = FALSE;
		}
	}
}

void CloseGamesv(){
	close(sockGamesv);
}

void CloseMsgsv(){
	close(sockMsgsv);
}

void CloseaLoginsv(){
	close(sockLoginsv);
}
