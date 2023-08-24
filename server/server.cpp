#include "DATA.h"
#include "mainFeatures.h"
#include "initSock.h"
#include "helper.h"



#include <cstdio>





BOOL successInitSock = FALSE;



int pubSize = 0;
PUCHAR pBuf = NULL;  // 0
// encryption def
UCHAR g_AES_oldKey[AES_KEY_IV_SIZE] = {};
UCHAR g_AES_oldIv[AES_KEY_IV_SIZE] = {};

int main() {

	// init rsa
	InitRSA();
	// store the old key, use the old key to encrypt the rsa first and then send the rsa pubkey to client  in fn AddSock()
	if (g_ClientCount == 0) {
		memcpy(g_AES_oldKey, GetAESKey(), AES_KEY_IV_SIZE);
		memcpy(g_AES_oldIv, GetAESIv(), AES_KEY_IV_SIZE);
	}
	SetAESKeyAndIv(g_AES_oldKey, g_AES_oldIv);
	pubSize = GetPubBuf(&pBuf);
	

	successInitSock = InitSock();
	if (successInitSock == FALSE) {
		while (true) {
			printf("waiting for connection from client.. \n");
			Sleep(2000);
			printf("still don't get response from client after 2s..\n");

			g_client = InitSock();
			printf("server.cpp  line 24: g_client: %d\n", g_client);
			if (g_client == 0) {
				continue;
			}

			printf("established connection with client.\n");
			break;
		}
	}
	UserInputCommand();
	

	
	
	WSACleanup();
	exit(0);
	return 0;
}