#include <WinSock2.h>
#include <stdio.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include "mainFeatures.h"


BOOL InitWinSock() {
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wd = {};
	int nResult = 0;
	nResult = WSAStartup(wVersionRequested, &wd);

	if (nResult == SOCKET_ERROR) {
		printf("initWinSock failed: %d\n", WSAGetLastError());
		return FALSE;
	}
	return TRUE;
}


BOOL InitSock() {
	InitWinSock();
	int nErrCode = 0;

	// 1. socket()
	SOCKET ListenSocket = socket(
		AF_INET,      //  IPv4
		SOCK_STREAM, //  TCP
		IPPROTO_TCP //  TCP
	);

	// check if creating socket successfully
	if (INVALID_SOCKET == ListenSocket) {
		printf("socket creation failed: %d\n", WSAGetLastError());
		WSACleanup();
		return FALSE;
	}


	// 2. initialize ip addr
	SOCKADDR_IN si = {};
	si.sin_family = AF_INET;
	si.sin_port = htons(12345);
	//si.sin_addr.S_un.S_addr = inet_addr("192.168.2.26");
	inet_pton(AF_INET, IP, &si.sin_addr);

	// 3. bind()
	nErrCode = bind(
		ListenSocket,
		(SOCKADDR*)&si,
		sizeof(si)
	);

	if (nErrCode == SOCKET_ERROR) {
		printf("bind failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return FALSE;
	}


	// 4. listen
	nErrCode = listen(
		ListenSocket,		// socket created in step 1
		SOMAXCONN		// SOMAXCONN
	);
	if (nErrCode == SOCKET_ERROR) {
		printf("listen failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return FALSE;
	}
	// End of testing for winsock listening


	CreateThread(NULL, NULL,
		AcceptProc, (LPVOID)ListenSocket,
		NULL, NULL);

	return true;
}