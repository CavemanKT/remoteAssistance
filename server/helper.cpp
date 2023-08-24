#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "15pb/RemoteControlAssistance.lib")
#include <stdio.h>

#include "DATA.h"
#include "mainFeatures.h"
#include "helper.h"





int g_newClientIndex = 0;


void AddSock(SOCKET ClientSock) {
	if (g_ClientCount == 0) {
		g_client = ClientSock;
		g_curClientIndex = 0;
		g_newClientIndex = 0;
	}

	SetAESKeyAndIv(g_AES_oldKey, g_AES_oldIv);

	// send pubKey
	SendMsgInfo(ClientSock, KEY, pubSize);			// pair 1 data transferring -> recvProc
	SendMsgData(ClientSock, (char*)pBuf, pubSize);
	for (int i = 0; i < LISTEN_COUNT; i++)
	{
		if (g_ClientSocks[i].client == 0) {    // everytime it loops to search for an empty slot for client.
			g_ClientSocks[i].client = ClientSock;
			g_newClientIndex = i;
			g_ClientCount++;
			printf("g_ClientSocks[i].client = %d, g_curClientIndex= %d, newClientIndex= %d\n", g_ClientSocks[i].client, g_curClientIndex, g_newClientIndex);
			break;
		}

	}
}

void DeleteSock(int g_curClientIndex) {
	g_ClientCount--;
	closesocket(g_ClientSocks[g_curClientIndex].client);
	WSACleanup();
	g_ClientSocks[g_curClientIndex].client = 0;
	memset(g_ClientSocks[g_curClientIndex].aesKey, NULL, sizeof(g_ClientSocks[g_curClientIndex].aesKey));
	memset(g_ClientSocks[g_curClientIndex].aesIv, NULL, sizeof(g_ClientSocks[g_curClientIndex].aesIv));

	if (g_ClientCount == 0) {
		exit(0);
		return;
	}
	else {
		
		if (g_curClientIndex - 1 > 0 && g_curClientIndex - 1 < 5) {
			g_curClientIndex -= 1;
			g_client = g_ClientSocks[g_curClientIndex].client;
		}
		else {
			g_curClientIndex += 1;
			g_client = g_ClientSocks[g_curClientIndex].client;
		}
		printf("you have changed to session %d.\n", g_curClientIndex);
	}
}


//done 
void SendMsgInfo(SOCKET client, MSG_TYPE type, int msgSize, char* pArg, int argSize) {

	char* pBuf[20] = {};    // need to resize the memory
	MSG_INFO msgInfo = {};
	msgInfo.type = type;
	msgInfo.size = msgSize;
	msgInfo.count = GET_COUNT(msgSize, DATA_SIZE);
	if (pArg != NULL) {
		memcpy(msgInfo.arg, pArg, argSize);
	}
	
	AESEncrpty((PCUCHAR)&msgInfo, sizeof(MSG_INFO), (PUCHAR)&msgInfo);

	send(
		client,
		(char*)&msgInfo,  // do we send pBuf or msgInfo
		sizeof(MSG_INFO),
		0);
}
//done

void SendMsgData(SOCKET client, char* pData, int dataSize) {
	DATA_INFO dataInfo = {};
	dataInfo.size = dataSize;
	memcpy(dataInfo.buff, pData, dataSize);
	AESEncrpty((PCUCHAR)&dataInfo, sizeof(DATA_INFO), (PUCHAR)&dataInfo);

	send(
		client,
		(char*)&dataInfo,
		sizeof(DATA_INFO),
		0
	);
}


void cmdCleaner() {
	memset(g_command, 0, sizeof(g_command));
	memset(g_arg1, 0, sizeof(g_arg1));
	memset(g_arg2, 0, sizeof(g_arg2));
}