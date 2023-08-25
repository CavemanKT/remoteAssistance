#pragma once
#include "rca.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define CMD_UPLOAD "upload"
#define CMD_DOWLOAD "download"
#define CMD_SYSTEM "system"
#define CMD_SCREENSHOT "screenshot"
#define CMD_CHANGE "change"
#define CMD_QUIT "QUIT"

#define PARMTER_SIZE 276
#define DATA_SIZE 1020

#define IP "127.0.0.1"
#define PORT 12345

enum MSG_TYPE {
	OTHER,
	UPLOAD,
	DOWNLOAD,
	SYS,
	SCREEN,
	CHANGE,   // for encryption cmd
	QUIT,
	KEY
};

typedef struct _MSG_INFO {
	MSG_TYPE type;
	int count;
	int size;
	char arg[MAX_PATH];
}MSG_INFO, *PMSG_INFO;

typedef struct _DATA_INFO {
	int size;
	char buff[DATA_SIZE];
}DATA_INFO, *PDATA_INFO;

#define GET_COUNT(size, alignment) size % alignment ? size/alignment + 1: size / alignment
extern SOCKET g_client;
extern char g_command[MAX_PATH];
extern char g_arg1[MAX_PATH];   // 1.txt	
extern char g_arg2[MAX_PATH];   // 2.txt    destination path


typedef struct _CLIENT_INFO {
	SOCKET client;
	sockaddr_in addr;
	UCHAR aesKey[AES_KEY_IV_SIZE];
	UCHAR aesIv[AES_KEY_IV_SIZE];
}CLIENT_INFO, *PCLIENT_INFO;

#define LISTEN_COUNT 5
extern CLIENT_INFO g_ClientSocks[LISTEN_COUNT];
extern int g_curClientIndex;  // current client
extern int g_ClientCount;	  // current amount of connected clients
extern int g_newClientIndex;

extern BOOL successInitSock;



// encryption  data
extern UCHAR g_AES_oldKey[AES_KEY_IV_SIZE];
extern UCHAR g_AES_oldIv[AES_KEY_IV_SIZE];
extern int pubSize;
extern PUCHAR pBuf;



////rsa length
//#define RSA_KEY_LENGTH 2048
//// rsa data
//extern int pri_len;  // 0
//extern int	pub_len; // 0
//extern char* pri_key; // NULL
//extern char* pub_key; // NULL
//
//// encrypted struct
//typedef struct _ENCRYPTINFO
//{
//	// length after encryption
//	int len;
//	// encrypted content, a pointer
//	PUCHAR buff;
//}ENCRYPTINFO, * PENCRYPTINFO;
//
//// decrypted struct
//typedef struct _DECRYPTINFO
//{
//	// length after decryption
//	int len;
//	// decrypted content , a pointer
//	PUCHAR buff;
//}DECRYPTINFO, * PDECRYPTINFO;
//
//// struct that saves the public key
//typedef struct _PUBLICKEYINFO
//{
//	// pubKey length
//	int len;
//	// pubKey content, a pointer
//	char* buff;
//}PUBLICKEYINFO, * PPUBLICKEYINFO;