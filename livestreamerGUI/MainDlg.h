// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
	public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_MYMSG, OnMyMsg)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_BUTTON_CLOSEB, BN_CLICKED, OnBnClickedButtonCloseb)
		COMMAND_HANDLER(IDC_BUTTON_GETB, BN_CLICKED, OnBnClickedButtonGetb)
		COMMAND_HANDLER(IDC_BUTTON_GETC, BN_CLICKED, OnBnClickedButtonGetc)
		COMMAND_HANDLER(IDC_BUTTON_CLOSEC, BN_CLICKED, OnBnClickedButtonClosec)
		COMMAND_HANDLER(IDC_BUTTON_GETD, BN_CLICKED, OnBnClickedButtonGetd)
		COMMAND_HANDLER(IDC_BUTTON_CLOSED, BN_CLICKED, OnBnClickedButtonClosed)
		COMMAND_HANDLER(IDC_BUTTON_GETE, BN_CLICKED, OnBnClickedButtonGete)
		COMMAND_HANDLER(IDC_BUTTON_CLOSEE, BN_CLICKED, OnBnClickedButtonClosee)
		COMMAND_HANDLER(IDC_BUTTON_ROOMID, BN_CLICKED, OnBnClickedButtonRoomid)
		COMMAND_HANDLER(IDC_BUTTON_VEDIOA, BN_CLICKED, OnBnClickedButtonVideoa)
		COMMAND_HANDLER(IDC_BUTTON_VEDIOB, BN_CLICKED, OnBnClickedButtonVediob)
		COMMAND_HANDLER(IDC_BUTTON_VEDIOC, BN_CLICKED, OnBnClickedButtonVedioc)
		COMMAND_HANDLER(IDC_BUTTON_VEDIOD, BN_CLICKED, OnBnClickedButtonVediod)
		COMMAND_HANDLER(IDC_BUTTON_VEDIOE, BN_CLICKED, OnBnClickedButtonVedioe)
	END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMyMsg(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);

	void CloseDialog(int nVal);
	LRESULT OnBnClickedButtonCloseb(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonGetb(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonGetc(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonClosec(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonGetd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonClosed(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonGete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonClosee(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

public:
	void CMainDlg::StartGetVedio(int iIdx, int iEdit, int iGetBtn, int iCloseBtn);
	void CMainDlg::CloseGetVedio(int iIdx, int iEdit, int iGetBtn, int iCloseBtn);
	void CMainDlg::SetRoomID(int RoomID);
	void CMainDlg::StartViewVedio(int iIdx, int iEdit, int iVedioBtn);
	BOOL CMainDlg::formClipboardData(wstring & strData);
public:
	LRESULT OnBnClickedButtonRoomid(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonVideoa(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonVediob(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonVedioc(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonVediod(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonVedioe(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
