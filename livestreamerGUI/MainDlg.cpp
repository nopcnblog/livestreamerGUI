// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include "Thread.h"

tagMyInfo MyInfo[6];
HWND MainHwnd;

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	UIUpdateChildWindows();
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	MainHwnd = m_hWnd;
	::EnableWindow(::GetDlgItem(m_hWnd, ID_APP_ABOUT), FALSE);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_CLOSEB), FALSE);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_CLOSEC), FALSE);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_CLOSED), FALSE);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_CLOSEE), FALSE);

	for (int i = 0; i < 5; i++)
	{
		MyInfo[i].bUnUsing = true;
	}
	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

void CMainDlg::StartGetVedio(int iIdx, int iEdit, int iGetBtn, int iCloseBtn)
{
	MyInfo[iIdx].iStartBtn = iGetBtn;
	MyInfo[iIdx].iCloseBtn = iCloseBtn;
	MyInfo[iIdx].bUnUsing = false;
	::EnableWindow(::GetDlgItem(m_hWnd, iGetBtn), FALSE);
	//::EnableWindow(::GetDlgItem(m_hWnd, iCloseBtn), TRUE);
	MyInfo[iIdx].RoomID.clear();
	TCHAR strTm[20] = {};
	::GetWindowText(::GetDlgItem(m_hWnd, iEdit), strTm, 20);
	MyInfo[iIdx].RoomID = strTm;
	if (MyInfo[iIdx].RoomID.empty())
	{
		MyInfo[iIdx].bUnUsing = true;
		::EnableWindow(::GetDlgItem(m_hWnd, iGetBtn), TRUE);
		::EnableWindow(::GetDlgItem(m_hWnd, iCloseBtn), FALSE);
		return;
	}

	MyInfo[iIdx].iButtonIdx = iIdx;

	HANDLE hTmp = (HANDLE)_beginthreadex(NULL, 0,
		GetVedio, (void*)&MyInfo[iIdx], 0, NULL);
	if (hTmp)
		CloseHandle(hTmp);
}

void CMainDlg::CloseGetVedio(int iIdx, int iEdit, int iGetBtn, int iCloseBtn)
{
	::TerminateProcess(MyInfo[iIdx].hProcess, 4);
}

void CMainDlg::SetRoomID(int RoomID)
{
	int EditIdx = -1;
	switch (RoomID)
	{
	case 0:
		EditIdx = IDC_EDIT_A;
		break;
	case 1:
		EditIdx = IDC_EDIT_B;
		break;
	case 2:
		EditIdx = IDC_EDIT_C;
		break;
	case 3:
		EditIdx = IDC_EDIT_D;
		break;
	case 4:
		EditIdx = IDC_EDIT_E;
		break;
	}
	if (EditIdx > 0)
	{
		::SetWindowText(::GetDlgItem(MainHwnd, EditIdx), MyInfo[RoomID].RoomID.c_str());
	}

}

void CMainDlg::StartViewVedio(int iIdx, int iEdit, int iVedioBtn)
{
	MyInfo[iIdx].iVedioBtn = iVedioBtn;
	::EnableWindow(::GetDlgItem(m_hWnd, iVedioBtn), FALSE);

	MyInfo[iIdx].RoomID.clear();
	TCHAR strTm[20] = {};
	::GetWindowText(::GetDlgItem(m_hWnd, iEdit), strTm, 20);
	MyInfo[iIdx].RoomID = strTm;
	if (MyInfo[iIdx].RoomID.empty())
	{
		MyInfo[iIdx].bUnUsing = true;
		::EnableWindow(::GetDlgItem(m_hWnd, iVedioBtn), TRUE);
		return;
	}
	MyInfo[iIdx].iButtonIdx = iIdx;
	HANDLE hTmp = (HANDLE)_beginthreadex(NULL, 0,
		ViewVedio, (void*)&MyInfo[iIdx], 0, NULL);
	if (hTmp)
		CloseHandle(hTmp);

}

LRESULT CMainDlg::OnMyMsg(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	int iCase = (int)lParam;
	switch (iCase)
	{
	case MSG00_EnableCloseBtn:
		::EnableWindow(::GetDlgItem(MainHwnd, MyInfo[(int)wParam].iCloseBtn), TRUE);
		break;
	case MSG01_EnableStartBtn:
		MyInfo[(int)wParam].bUnUsing = true;
		::EnableWindow(::GetDlgItem(MainHwnd, MyInfo[(int)wParam].iStartBtn), TRUE);
		::EnableWindow(::GetDlgItem(MainHwnd, MyInfo[(int)wParam].iCloseBtn), FALSE);
		FlashWindow(TRUE);
		break;
	case MSG02_SetRoomID:
		SetRoomID((int)wParam);
		break;
	case MSG03_EnableVedioBtn:
		::EnableWindow(::GetDlgItem(MainHwnd, MyInfo[(int)wParam].iVedioBtn), TRUE);
		break;
	}

	return 0;
}


//CloseA
LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseGetVedio(0, NULL, NULL, NULL);
	return 0;
}


//GetA
LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	StartGetVedio(0, IDC_EDIT_A, IDOK, ID_APP_ABOUT);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(0);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}


LRESULT CMainDlg::OnBnClickedButtonCloseb(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	CloseGetVedio(1, IDC_EDIT_B, IDC_BUTTON_GETB, IDC_BUTTON_CLOSEB);
	return 0;
}


LRESULT CMainDlg::OnBnClickedButtonGetb(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	StartGetVedio(1, IDC_EDIT_B, IDC_BUTTON_GETB, IDC_BUTTON_CLOSEB);
	return 0;
}


LRESULT CMainDlg::OnBnClickedButtonGetc(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	StartGetVedio(2, IDC_EDIT_C, IDC_BUTTON_GETC, IDC_BUTTON_CLOSEC);
	return 0;
}


LRESULT CMainDlg::OnBnClickedButtonClosec(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	CloseGetVedio(2, IDC_EDIT_C, IDC_BUTTON_GETC, IDC_BUTTON_CLOSEC);
	return 0;
}


LRESULT CMainDlg::OnBnClickedButtonGetd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	StartGetVedio(3, IDC_EDIT_D, IDC_BUTTON_GETD, IDC_BUTTON_CLOSED);
	return 0;
}


LRESULT CMainDlg::OnBnClickedButtonClosed(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	CloseGetVedio(3, IDC_EDIT_D, IDC_BUTTON_GETD, IDC_BUTTON_CLOSED);
	return 0;
}


LRESULT CMainDlg::OnBnClickedButtonGete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	StartGetVedio(4, IDC_EDIT_E, IDC_BUTTON_GETE, IDC_BUTTON_CLOSEE);
	return 0;
}


LRESULT CMainDlg::OnBnClickedButtonClosee(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	CloseGetVedio(4, IDC_EDIT_E, IDC_BUTTON_GETE, IDC_BUTTON_CLOSEE);
	return 0;
}


LRESULT CMainDlg::OnBnClickedButtonRoomid(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	wstring szUrl;
	formClipboardData(szUrl);
	if (szUrl.empty())
	{
		return 0;
	}
	::SetWindowText(::GetDlgItem(m_hWnd, IDC_EDIT_URL), szUrl.c_str());
	pMyInfo sinfo = new tagMyInfo;
	wstringToString(szUrl, sinfo->szUrl);

	HANDLE hTmp = (HANDLE)_beginthreadex(NULL, 0,
		GetRoomID, sinfo, 0, NULL);
	if (hTmp)
		CloseHandle(hTmp);
	return 0;
}


BOOL CMainDlg::formClipboardData(wstring & strData)
{
	//判断剪贴板的数据格式是否可以处理。
	if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
	{
		return FALSE;
	}
	if (!OpenClipboard())
	{
		return FALSE;
	}
	HGLOBAL hMem = GetClipboardData(CF_UNICODETEXT);
	if (hMem != NULL)
	{
		LPTSTR lpStr = (LPTSTR)GlobalLock(hMem);
		if (lpStr != NULL)
		{
			wstring DataTmp(lpStr);
			strData = L"";
			int s = DataTmp.at(1);
			for (UINT i = 0; i < DataTmp.length(); i++)//去标点符号
			{
				s = DataTmp.at(i);
				if (s != ' ')
					strData += DataTmp.at(i);
			}
			//OutputDebugString(lpStr);
			GlobalUnlock(hMem);
		}
	}
	else return FALSE;
	CloseClipboard();
	return TRUE;
}

LRESULT CMainDlg::OnBnClickedButtonVideoa(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	StartViewVedio(0, IDC_EDIT_A, IDC_BUTTON_VEDIOA);
	return 0;
}


LRESULT CMainDlg::OnBnClickedButtonVediob(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	StartViewVedio(1, IDC_EDIT_B, IDC_BUTTON_VEDIOB);
	return 0;
}


LRESULT CMainDlg::OnBnClickedButtonVedioc(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	StartViewVedio(2, IDC_EDIT_C, IDC_BUTTON_VEDIOC);
	return 0;
}


LRESULT CMainDlg::OnBnClickedButtonVediod(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	StartViewVedio(3, IDC_EDIT_D, IDC_BUTTON_VEDIOD);
	return 0;
}


LRESULT CMainDlg::OnBnClickedButtonVedioe(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	StartViewVedio(4, IDC_EDIT_E, IDC_BUTTON_VEDIOE);
	return 0;
}
