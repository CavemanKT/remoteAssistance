#pragma once
#include "DATA.h"

MESSAGEINFO AnalazyMessage(int size);
void SendStr(SOCKET client, char* str, int strSize);
void SendMsgInfo(SOCKET client, MSG_TYPE type, int msgSize, char* pArg = NULL, int argSize = 0);
void SendMsgData(SOCKET client, char* pData, int dataSize);
BOOL HandleCD(char* buff);