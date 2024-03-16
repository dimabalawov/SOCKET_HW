#define _CRT_SECURE_NO_WARNINGS
#define MAX_SIZE 255

#include <iostream>
#include <string>
#include "WinSock2.h" 
#include <ws2tcpip.h> 
#include "ClientDlg.h"
#pragma comment(lib, "Ws2_32.lib") 

using namespace std;

CClientDlg* CClientDlg::ptr = nullptr;

WSADATA wsaData;
SOCKET _socket;
sockaddr_in addr;

void CreateClientSocket(HWND hwnd) {
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    addr.sin_port = htons(20000);
    connect(_socket, (SOCKADDR*)&addr, sizeof(addr));

    char buff[MAX_SIZE], buff2[MAX_SIZE];
    int i = recv(_socket, buff, MAX_SIZE, 0);
    buff[i] = '\0';
    SendMessageA(CClientDlg::ptr->hEditReadOnly, EM_SETSEL, -1, -1);
    SendMessageA(CClientDlg::ptr->hEditReadOnly, EM_REPLACESEL, TRUE, (LPARAM)buff);
    SendMessageA(CClientDlg::ptr->hEditReadOnly, EM_REPLACESEL, TRUE, (LPARAM)"   ");
    GetWindowTextA(CClientDlg::ptr->hMessage, buff2, strlen(buff2));
    send(_socket, buff2, strlen(buff2), 0);
    system("pause");

}


CClientDlg::CClientDlg(void) {
    ptr = this;
}

CClientDlg::~CClientDlg(void) {
}

void CClientDlg::Cls_OnClose(HWND hwnd) {
    EndDialog(hwnd, IDCANCEL);
}

BOOL CClientDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
    hConnect = GetDlgItem(hwnd, IDCONNECT);
    hSend = GetDlgItem(hwnd, IDC_SEND);
    hEditReadOnly = GetDlgItem(hwnd, IDC_READONLY);
    hMessage = GetDlgItem(hwnd, IDC_MESSAGE);
    return TRUE;
}

void CClientDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
    TCHAR text[200];
    if (id == IDCONNECT) {
        CreateClientSocket(hwnd);
    }
    else if (id == IDC_END) {
        closesocket(_socket);
        WSACleanup();
    }
}

BOOL CALLBACK CClientDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
        HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
    }
    return FALSE;
}
