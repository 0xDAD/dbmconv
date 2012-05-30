#include "StdAfx.h"
#include "MainDlg.h"


LRESULT CMainDlg::OnBnClickedButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{	
	LPCTSTR pszDefExt = _T("mdb");
	CFileDialog dlg(TRUE, pszDefExt, NULL, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST, L"Справочник (*.mdb)\0*.mdb\0");
	INT_PTR nRes = dlg.DoModal();
	ATLASSERT(nRes > 0);
	if (nRes != IDOK)
		return 0;
	Options::SetInFileName(CString(dlg.m_szFileName));
	UpdatePage();
	return 0;
}


LRESULT CMainDlg::OnBnClickedButton3(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LPCTSTR pszDefExt = _T("xml");
	CFileDialog dlg(FALSE, pszDefExt, NULL, 0, L"Справочник 2.0 (*.xml)\0*.xml");
	INT_PTR nRes = dlg.DoModal();
	ATLASSERT(nRes > 0);
	if (nRes != IDOK)
		return 0;
	Options::SetOutFileName(CString(dlg.m_szFileName));
	UpdatePage();
	return 0;
}

void CMainDlg::UpdatePage()
{
	SetDlgItemText(IDC_EDIT1, Options::GetInFileName());
	SetDlgItemText(IDC_EDIT2, Options::GetOutFileName());
	//GetDlgItem(IDC_EDIT1)->SetWindowText(Options::GetInFileName());
	//GetDlgItem(IDC_EDIT2)->SetWindowText(Options::GetOutFileName());
}

LRESULT CMainDlg::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CloseDialog(0);

	return 0;
}


LRESULT CMainDlg::OnBnClickedButton2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CNamespace::ImportDictionary(Options::GetInFileName());
	
	return 0;
}

LRESULT CMainDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
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
	UpdatePage();
	return TRUE;
}

LRESULT CMainDlg::OnDestroy( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

void CMainDlg::CloseDialog( int nVal )
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

BOOL CMainDlg::PreTranslateMessage( MSG* pMsg )
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}