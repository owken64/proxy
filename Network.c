#include "Network.h"
#include "Parameter.h"
#include "Bool.h"
#include "Config.h"

#include <stdio.h> //printf(), fprintf(), perror()
#include <sys/socket.h> //socket(), bind(), accept(), listen()
#include <arpa/inet.h> // struct sockaddr_in, struct sockaddr, inet_ntoa()
#include <stdlib.h> //atoi(), exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <string.h> //memset()
#include <unistd.h> //close()
#include <sys/select.h>

#define QUEUELIMIT 5

int ConnectClient();
int ConnectGamesv();
int ConnectMsgsv();
int ConnectLoginsv();
void CloseClient();
void CloseGamesv();
void CloseMsgsv();
void CloseLoginsv();

int NetworkInit(){
	
	// パラメータの初期化
	for (int i = 0; i < MAX_CONNECTION; i++){
	  availableSockClient[i] = FALSE;
	  flgMsgRecvClient[i] = FALSE;
	  flgMsgSendClient[i] = FALSE;
	}
	flgMsgRecvGamesv = FALSE;
	flgMsgRecvMsgsv = FALSE;
	flgMsgRecvLoginsv = FALSE;
	flgMsgSendGamesv = FALSE;
	flgMsgSendMsgsv = FALSE;
	flgMsgSendLoginsv = FALSE;
	
	FD_ZERO(&readfds);
	
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

int NetworkReceive(){
	fd_set fds;
    unsigned int clitLen; // client internet socket address length

	// fd setの初期化
	memcpy(&fds, &readfds, sizeof(fd_set));

	// fdsに設定されたソケットが読み込み可能になるまで待ちます
	select(0, &fds, NULL, NULL, NULL);

	// listenしているサーバで受信した場合
	if (FD_ISSET(sockServer, &fds)){
		int idx;
		// 空いているソケットを探す
		idx = 0;
		while ( (idx < MAX_CONNECTION) & (availableSockClient[idx] == TRUE)) idx++;
		if (idx == MAX_CONNECTION) {
			perror("socket is full.");
			return NETWORK_RECEIVE_FAILURE;
		}
	
    	clitLen = sizeof(sockClient[idx]);
    	// Accept
    	if ((sockClient[idx] = accept(sockServer, (struct sockaddr *) &sockAddrClient[idx], &clitLen)) < 0) {
    	    perror("accept() failed.");
    	    return NETWORK_RECEIVE_FAILURE;
    	}
    	FD_SET(sockClient[idx], &readfds);
    	availableSockClient[idx] = TRUE;
    }
	
	for (int i = 0; i < MAX_CONNECTION; i++){
		if (FD_ISSET(sockClient[i], &fds)){
				// sockProxyからデータを受信します
				memset(messageRecvClient[i], 0, sizeof(messageRecvClient[i]));
				recv(sockClient[i], messageRecvClient[i], sizeof(messageRecvClient[i]), 0);
				flgMsgRecvClient[i] = TRUE;
		}
	}

	if (FD_ISSET(sockGamesv, &fds)){
			// sockProxyからデータを受信します
			memset(messageRecvGamesv, 0, sizeof(messageRecvGamesv));
			recv(sockGamesv, messageRecvGamesv, sizeof(messageRecvGamesv), 0);
			flgMsgRecvGamesv = TRUE;
	}

	if (FD_ISSET(sockMsgsv, &fds)){
			// sockProxyからデータを受信します
			memset(messageRecvMsgsv, 0, sizeof(messageRecvMsgsv));
			recv(sockMsgsv, messageRecvMsgsv, sizeof(messageRecvMsgsv), 0);
			flgMsgRecvMsgsv = TRUE;
	}

	if (FD_ISSET(sockLoginsv, &fds)){
			// sockProxyからデータを受信します
			memset(messageRecvLoginsv, 0, sizeof(messageRecvLoginsv));
			recv(sockLoginsv, messageRecvLoginsv, sizeof(messageRecvLoginsv), 0);
			flgMsgRecvLoginsv = TRUE;
	}
	
	return NETWORK_RECEIVE_SUCCESS;
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

	FD_SET(sockServer, &readfds);

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
 
 	FD_SET(sockGamesv, &readfds);
 
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

	FD_SET(sockMsgsv, &readfds);

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

	FD_SET(sockLoginsv, &readfds);

	return NETWORK_CONNECT_SUCCESS;
}

void CloseClient(){
	for (int i = 0; i < MAX_CONNECTION; i++){
		if (availableSockClient[i]) {
			FD_CLR(sockClient[i], &readfds);
			close(sockClient[i]);
			availableSockClient[i] = FALSE;
		}
	}
	FD_CLR(sockServer, &readfds);
	close(sockServer);
}

void CloseGamesv(){
	FD_CLR(sockGamesv, &readfds);
	close(sockGamesv);
}

void CloseMsgsv(){
	FD_CLR(sockMsgsv, &readfds);
	close(sockMsgsv);
}

void CloseaLoginsv(){
	FD_CLR(sockLoginsv, &readfds);
	close(sockLoginsv);
}
