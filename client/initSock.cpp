#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Shlwapi.lib")
#include <stdio.h>
#include <WS2tcpip.h>

#include "DATA.h"

SOCKET InitSocket()
{

	WORD wVersion = MAKEWORD(2, 2);
	WSADATA wd = {};
	int err = WSAStartup(wVersion, &wd);
	if (err != 0)
	{
		printf("fail to initialize WSASTARTUP");
		return 0;
	}


	SOCKET ConnectSocket = socket(
		AF_INET,
		SOCK_STREAM,

		IPPROTO_TCP
	);

	if (INVALID_SOCKET == ConnectSocket)
	{
		printf("fail to create socket");
		WSACleanup();
		return 0;
	}

	SOCKADDR_IN si = {};
	si.sin_family = AF_INET;
	si.sin_port = htons(PORT);
	inet_pton(AF_INET, IP, &si.sin_addr.S_un);
	err = connect(
		ConnectSocket,
		(sockaddr*)&si,
		sizeof(si)
	);
	if (0 != err)
	{
		printf("fail to connect the server");
		closesocket(ConnectSocket);
		WSACleanup();
		return 0;
	}

	return ConnectSocket;
}
