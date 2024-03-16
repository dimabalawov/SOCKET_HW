#pragma once
#include "header.h"

class CClientDlg
{
public:
	CClientDlg(void);
	~CClientDlg(void);

	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static CClientDlg* ptr;

	void Cls_OnClose(HWND hwnd);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	HWND hEditReadOnly, hMessage, hConnect, hSend;
};
