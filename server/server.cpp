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
	
	RECONNECT:
	successInitSock = InitSock();
	if (successInitSock == FALSE) {
		printf("waiting for connection from client.. \n");
		Sleep(1000);
		printf("still don't get response from client after 2s..\n");

			
		printf("server.cpp  line 44: g_client: %d\n", g_client);
		goto RECONNECT;

		g_client = successInitSock;
		printf("established connection with client.\n");
	}
	UserInputCommand();
	

	
	
	WSACleanup();
	exit(0);
	return 0;
}