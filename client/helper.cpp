#include <WinSock2.h>

#include "DATA.h"
#include <stdio.h>
#include <stdlib.h>
#include "rca.h"


MESSAGEINFO AnalazyMessage(int size)
{
	MESSAGEINFO message = {};

	message.count = size / DATA_SIZE + 1;

	message.size = size % DATA_SIZE;

	if (message.size == 0)
	{
		message.count -= 1;
	}
	return message;

}


void SendStr(SOCKET client, char* str, int strSize) {
	AESEncrpty((PCUCHAR)str, strSize, (PUCHAR)str);	
	send(client,(char*)str, (int)strSize, 0);
}

//done 
void SendMsgInfo(SOCKET client, MSG_TYPE type, int msgSize, char* pArg = NULL, int argSize = 0) {

	char* pBuf[20] = {};    // need to resize the memory
	MESSAGEINFO msgInfo = {};
	msgInfo.type = type;
	msgInfo.size = msgSize;
	msgInfo.count = GET_COUNT(msgSize, DATA_SIZE);
	if (pArg != NULL) {
		memcpy(msgInfo.arg, pArg, argSize);
	}
	AESEncrpty((PCUCHAR)&msgInfo, sizeof(MESSAGEINFO), (PUCHAR)&msgInfo);
	send(
		client,
		(char*)&msgInfo,  // do we send pBuf or msgInfo
		sizeof(MESSAGEINFO),
		0);
}

//done
void SendMsgData(SOCKET client, char* pData, int dataSize) {
	MYDATAINFO dataInfo = {};
	dataInfo.size = dataSize;
	memcpy(dataInfo.buff, pData, dataSize);
	AESEncrpty((PCUCHAR)&dataInfo, sizeof(MYDATAINFO), (PUCHAR)&dataInfo);

	send(
		client,
		(char*)&dataInfo,
		sizeof(MYDATAINFO),
		0
	);
}


BOOL HandleCD(char* buff) {
	if (buff[0] == 'C' && buff[1] == 'D') {
		printf("\n     command cd is forbidden.		\n");
		return TRUE;
	}
	return FALSE;
}