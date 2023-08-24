#pragma once
#include <windows.h>

#define CMD_UPLOAD "upload"
#define CMD_DOWLOAD "download"
#define CMD_SYSTEM "system"
#define CMD_SCREENSHOT "screenshot"
#define CMD_CHANGE "change"
#define CMD_QUIT "QUIT"


#define IP "192.168.2.26"
#define PORT 12345


#define DATA_SIZE 1020

extern SOCKET g_ClientSock;

enum MSG_TYPE {
	OTHER,		// no use
	UPLOAD,
	DOWNLOAD,
	SYS,	
	SCREEN,		// screenshot
	CHANGE,	    // session feature
	QUIT,
	KEY			// encryption feature
};

typedef struct _MESSAGEINFO
{
	MSG_TYPE type;
	int count;
	int size;
	char arg[MAX_PATH];
}MESSAGEINFO, * PMESSAGEINFO;


typedef struct _MYDATAINFO
{
	int size;
	char buff[DATA_SIZE];
}MYDATAINFO, * MYPDATAINFO;

#define GET_COUNT(size, alignment) size % alignment ? size/alignment + 1: size / alignment
