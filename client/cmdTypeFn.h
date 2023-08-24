#pragma once
// kind of done, the limit of data transfer is 276
void DownloadFile(SOCKET ConnectSocket, char* path);
// not yet done, cannot terminate itself after upload
void UploadFile(SOCKET ConnectSocket, MESSAGEINFO* pMsg);
void SystemCmd();
void Screen(MESSAGEINFO* pMsg);
void ChangeSession(MESSAGEINFO* pMsg);
void KeyCmd();