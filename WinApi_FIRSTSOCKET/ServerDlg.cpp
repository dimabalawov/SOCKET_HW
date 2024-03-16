#define _CRT_SECURE_NO_WARNINGS
#define MAX_SIZE 255


#include <iostream>
#include <string>
#include "WinSock2.h" 
#include <ws2tcpip.h> 
#pragma comment(lib, "Ws2_32.lib") 
#include "ServerDlg.h"



using namespace std;

CServerDlg* CServerDlg::ptr = nullptr;


WSADATA wsaData;
SOCKET _socket;
SOCKET acceptSocket;
sockaddr_in addr;

void CreateServerSocket() {
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
    addr.sin_port = htons(20000);
    bind(_socket, (SOCKADDR*)&addr, sizeof(addr));
    listen(_socket, 1);
    acceptSocket = accept(_socket, NULL, NULL);
    char buff[MAX_SIZE];
    GetWindowTextA(CServerDlg::ptr->hEdit, buff, MAX_SIZE);
    send(acceptSocket, buff, strlen(buff) + 1, 0);
    int i = recv(acceptSocket, buff, MAX_SIZE, 0);
    buff[i] = '\0';
    SendMessageA(CServerDlg::ptr->hEditReadOnly, EM_SETSEL, -1, -1); 
    SendMessageA(CServerDlg::ptr->hEditReadOnly, EM_REPLACESEL, TRUE, (LPARAM)buff);
    SendMessageA(CServerDlg::ptr->hEditReadOnly, EM_REPLACESEL, TRUE, (LPARAM)"   ");
    WSACleanup();
    system("pause");
}

CServerDlg::CServerDlg(void)
{
    ptr = this;
}

CServerDlg::~CServerDlg(void)
{
}

void CServerDlg::Cls_OnClose(HWND hwnd)
{
    EndDialog(hwnd, IDCANCEL);
}

BOOL CServerDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
    hButtonStart = GetDlgItem(hwnd, IDC_START);
    hButtonEnd = GetDlgItem(hwnd, IDC_END);
    hEditReadOnly = GetDlgItem(hwnd, IDC_READONLY);
    hEdit = GetDlgItem(hwnd, IDC_EDIT1);

    return TRUE;
}

void CServerDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
    TCHAR textBuff[MAX_SIZE];
    TCHAR clientTextBuff[MAX_SIZE];
    if (id == IDC_START)
        CreateServerSocket();
    else if (id == IDC_END)
    {
        closesocket(acceptSocket);
        closesocket(_socket);
        WSACleanup();
    }
}

BOOL CALLBACK CServerDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
        HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
        HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
    }
    return FALSE;
};