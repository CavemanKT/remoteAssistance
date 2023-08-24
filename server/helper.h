
#pragma once
void AddSock(SOCKET ClientSock);
void DeleteSock(int g_curClientIndex);


//done 
void SendMsgInfo(SOCKET client, MSG_TYPE type, int msgSize, char* pArg = NULL, int argSize = 0);
//done
void SendMsgData(SOCKET client, char* pData, int dataSize);
void cmdCleaner();

