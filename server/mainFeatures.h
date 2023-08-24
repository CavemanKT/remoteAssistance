#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include "DATA.h"

void ExecCmd(SOCKET client, char* pCmd, char* pArg1, char* pArg2);
void AnalyzeCmd(SOCKET client, char* pCmd);


DWORD WINAPI RecvProc(LPVOID lpThreadParameter);
DWORD WINAPI AcceptProc(LPVOID lpThreadParameter);

void UserInputCommand();
