#pragma once
#include <windows.h>
#include <ctime>
#include <tchar.h>
#include<commctrl.h>
#include <windowsX.h>
#include "resource.h"

#pragma comment(lib,"comctl32")


using namespace std;

HWND hListBox1, hListBox2, hListBox3, hPlay1, hPlay2, hPlay3;
HANDLE Th1, Th2, Th3;

BOOL CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
	INITCOMMONCONTROLSEX icc = { sizeof(INITCOMMONCONTROLSEX) };
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

void Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}


DWORD WINAPI Thread(LPVOID lp)
{
	HWND hListBox = (HWND)lp;
	int maxVal = 50000;
	if (hListBox == hListBox1) {
		maxVal = 50000; 
	}
	else if (hListBox == hListBox2) {
		maxVal = 25000; 
	}
	else if (hListBox == hListBox3) {
		maxVal = 20000; 
	}

	while (TRUE)
	{
		int randNum = rand() % (maxVal + 1);
		if (hListBox == hListBox2 && randNum % 2 != 0) {
			continue;// Пропустить 
		}
		TCHAR buffer[10]; // буфер для преобразования числа в строку
		_itot_s(randNum, buffer, 10);
		SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)buffer); 
		Sleep(250);
	}
	return 0;
}




BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	srand(time(0));
	hListBox1 = GetDlgItem(hwnd, IDC_LIST1);


	hListBox2 = GetDlgItem(hwnd, IDC_LIST2);


	hListBox3 = GetDlgItem(hwnd, IDC_LIST3);
	

	hPlay1 = GetDlgItem(hwnd, IDC_PLAY1);
	hPlay2 = GetDlgItem(hwnd, IDC_PLAY2);
	hPlay3 = GetDlgItem(hwnd, IDC_PLAY3);

	Th1 = CreateThread(NULL, 0, Thread, hListBox1, CREATE_SUSPENDED, NULL);
	Th2 = CreateThread(NULL, 0, Thread, hListBox2, 0, NULL);
	Th3 = CreateThread(NULL, 0, Thread, hListBox3, 0, NULL);
	return TRUE;
}

void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_PLAY1)
	{
		static BOOL flag = FALSE;
		if (flag)
		{
			SuspendThread(Th1);
			SetWindowText(hPlay1, TEXT("Пуск"));
		}
		else
		{
			ResumeThread(Th1);
			SetWindowText(hPlay1, TEXT("Пауза"));
		}
		flag = !flag;
	}
	else if (id == IDC_PLAY2)
	{
		static BOOL flag = TRUE;
		if (flag)
		{
			SuspendThread(Th2);
			SetWindowText(hPlay2, TEXT("Пуск"));
		}
		else
		{
			ResumeThread(Th2);
			SetWindowText(hPlay2, TEXT("Пауза"));
		}
		flag = !flag;
	}
	else if (id == IDC_PLAY3)
	{
		static BOOL flag = TRUE;
		if (flag)
		{
			SuspendThread(Th3);
			SetWindowText(hPlay3, TEXT("Пуск"));
		}
		else
		{
			ResumeThread(Th3);
			SetWindowText(hPlay3, TEXT("Пауза"));
		}
		flag = !flag;
	}
	else if (id == IDC_STOP1)
	{
		TerminateThread(Th1, 0);
		CloseHandle(Th1);
		EnableWindow(hPlay1, FALSE);
	}
	else if (id == IDC_STOP2)
	{
		TerminateThread(Th2, 0);
		CloseHandle(Th2);
		EnableWindow(hPlay2, FALSE);
	}
	else if (id == IDC_STOP3)
	{
		TerminateThread(Th3, 0);
		CloseHandle(Th3);
		EnableWindow(hPlay3, FALSE);
	}
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		Cls_OnInitDialog(hwnd, (HWND)wParam, lParam);
		return TRUE;

	case WM_CLOSE:
		Cls_OnClose(hwnd);
		return TRUE;

	case WM_COMMAND:
		Cls_OnCommand(hwnd, LOWORD(wParam), (HWND)lParam, HIWORD(wParam));
		return TRUE;
	}
	return FALSE;
}
