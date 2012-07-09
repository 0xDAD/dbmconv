// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "UIMessages.h"
#include "MultiPaneStatusBarEx.h"
#include "UniListView.h"
#include "UniTreeView.h"
#include "WtlExResizePropertySheet.h"
#include "PropertyPageTagClass.h"



class CMainFrame : 
	public CFrameWindowImpl<CMainFrame>, 
	public CUpdateUI<CMainFrame>,
	public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

protected:
	CString m_strDocFileName;

	CCommandBarCtrl m_CmdBar;
	CMPSBarWithProgress m_wndStatusBar;
	CSplitterWindow m_wndSplitterVert;
	CHorSplitterWindow m_wndSplitterHorz;

	CPaneContainer m_wndPaneTree;
	CUniTreeView m_wndItemTree;

	CPaneContainer m_wndPaneList;
	CUniListView m_wndItemList;

	CPaneContainer m_wndPaneOutput;

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UIUpdateToolBar();
		UIUpdateStatusBar();
		return FALSE;
	}
public:
	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)

		UPDATE_ELEMENT(0, UPDUI_STATUSBAR)
		UPDATE_ELEMENT(1, UPDUI_STATUSBAR)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)

		MESSAGE_HANDLER(WM_MFRAME_NOTIFY, OnInternalNotify)

		COMMAND_ID_HANDLER(ID_POPUP_LOADMDB, OnLoadDBM)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_OPEN, OnFileOpen)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_FILE_SAVE_AS, OnFileSaveAs)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_ITEM_PROPS1, OnItemProps1)

		COMMAND_ID_HANDLER(ID_POPUP_CNV_MAKETAGS, OnMakeTagClasses)

		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)

		NOTIFY_CODE_HANDLER(LVN_ITEMCHANGED, OnItemChanged)
		//REFLECT_NOTIFY_CODE(TVN_SELCHANGED)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

protected:
	void _UpdateStatusBar(){
		UISetText(0, L"1");
		CString strSel;
		strSel.Format(ID_STATUSBARPANE_LVINFO, m_wndItemList.GetSelectedCount());
		UISetText(1, strSel);
	}
	void _UpdateTitle(){
		// Update title
		CString strTitle;
		ATLVERIFY(strTitle.LoadString(IDR_MAINFRAME));

		CString strTitleNew;
		if (m_strDocFileName.IsEmpty())
			strTitleNew = "dbmconv";
		else
		{
			TCHAR szPath[64] = { 0 };
			ATLVERIFY(AtlCompactPath(szPath, m_strDocFileName, _countof(szPath)));
			strTitleNew = szPath;
		}
		if (GetModel().IsModified())
			strTitleNew += _T("*");
		strTitleNew += _T(" - ");
		strTitleNew += strTitle;
		SetWindowText(strTitleNew);
	}
	bool _FileSave(LPCTSTR pszPath = NULL)
	{
		if (NULL == pszPath)
			pszPath = _T("");
		CString strFilePath;
		//GetNamespace().GetConfigFileName(strFilePath);
		//if (!GetNamespace().IsModified() && !strFilePath.IsEmpty() && strFilePath == pszPath)
		//	return true;

		CString strPath(pszPath);
		if (strPath.IsEmpty())
		{
			//CFileDialogFilter strFltr;
			//ATLVERIFY(strFltr.SetFilter(IDS_FILTER_MDB_XML));
			LPCTSTR pszDefExt = _T("xml");
			CFileDialog dlg(FALSE, pszDefExt, NULL, OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"XML Files\0*.xml");
			INT_PTR nRes = dlg.DoModal();
			ATLASSERT(nRes > 0);
			if (nRes != IDOK)
				return false;
			strPath = dlg.m_szFileName;
		}

		CWaitCursor wc;
		if (FAILED(GetModel().SaveToXML(strPath)))
		{
			AtlMessageBox(m_hWnd, L"Unable to save", IDR_MAINFRAME, MB_OK | MB_ICONSTOP);
			return false;
		}
		_UpdateTitle();
		return true;
	}
	bool _FileClose(){
		if(GetModel().IsModified()){
			int nRes =  AtlMessageBox(NULL, IDP_SAVE_CHANGES, IDS_DOC_CLOSING, MB_YESNOCANCEL);
			if (nRes == IDOK)
				GetModel().SaveToXML(m_strDocFileName);
			else if(nRes == IDNO)
				GetModel().ClearModel();
			else
				return false;
		}
			
		m_strDocFileName = L"";
		_UpdateTitle();
		return true;
	}
	bool _FileOpen(LPCTSTR pszPath = NULL){
		if(pszPath == NULL)
			pszPath = L"";
		CString strPath = pszPath;
		if(strPath.IsEmpty()){
			LPCTSTR pszDefExt = _T("xml");
			CFileDialog dlg(TRUE, pszDefExt, NULL, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST, L"XML-файлы (*.xml)\0*.xml\0");
			INT_PTR nRes = dlg.DoModal();
			ATLASSERT(nRes > 0);
			if (nRes != IDOK)
				return 0;	
			strPath = dlg.m_szFileName;
		}
		if(SUCCEEDED(GetModel().LoadFromXML(strPath))){			
			m_wndItemTree.InitView();
			m_wndItemList.SetParentId(ITEM_ID_ROOT);
			m_strDocFileName = strPath;
		}
		else{
			AtlMessageBox(NULL, L"Unknown error occured while parsing xml");
			return false;
		}
		_UpdateTitle();
		return true;
	}
protected:
	LRESULT OnItemProps1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CPropertySheetResize PropertySheet(L"Свойства");
		vector<int> vctSelIds;
		m_wndItemList.GetSelectedItemIds(vctSelIds);
		int nType = -1;		
		GetModel().GetMultiItemType(vctSelIds, nType);

		switch(nType){
		case ItemTypeTagClass:
			CPropertyPageTagClass TagClass(vctSelIds);
			PropertySheet.AddPage(TagClass);
			break;			
		}
		int nRes = PropertySheet.DoModal();
		
		return 0;
	}
	LRESULT OnInternalNotify(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/){
		int nItemId = (int)lParam;
		switch(wParam){
		case UpdateSelection:
			m_wndItemList.SetParentId(nItemId);
			_UpdateStatusBar();
			break;		
		}
		return 0;
	}


	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// create command bar window
		HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
		// attach menu
		m_CmdBar.AttachMenu(GetMenu());
		// load command bar images
		m_CmdBar.SetImageMaskColor(RGB(255, 0, 255));
		m_CmdBar.LoadImages(IDR_MAINFRAME);
		// remove old menu
		SetMenu(NULL);

		HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
		CToolBarCtrl ToolBarCtrl(hWndToolBar);
		ToolBarCtrl.SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

		// Create rebar window
		if (!CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE))
		{
			ATLTRACE("Failed to create rebar\n");
			return -1;
		}

		AddSimpleReBarBand(hWndCmdBar);
		AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

		// Create status bar window
		if (!CreateSimpleStatusBar(ATL_IDS_IDLEMESSAGE, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SBARS_TOOLTIPS))
		{
			ATLTRACE("Failed to create status bar\n");
			return -1;
		}
		m_wndStatusBar.SubclassWindow(m_hWndStatusBar);
		int nArrayStatusBarParts[] =
		{
			ID_DEFAULT_PANE,
			ID_STATUSBARPANE_LVINFO,
			ID_STATUSBARPANE_PROG
			/*
			ID_STATUSBAR_PANE_TRANSACTIONS,
			ID_STATUSBAR_PANE_CHANNELS,*/
		};
		m_wndStatusBar.SetPanes(nArrayStatusBarParts, _countof(nArrayStatusBarParts), false);
		/*CString strToolTip;
		ATLVERIFY(strToolTip.LoadString(IDS_STATUS_SERVER));
		m_wndStatusBar.SetTipText(1, strToolTip);*/

		UIAddToolBar(hWndToolBar);
		UIAddStatusBar(m_hWndStatusBar);

		UISetCheck(ID_VIEW_TOOLBAR, 1);
		UISetCheck(ID_VIEW_STATUS_BAR, 1);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		// Prepare horizontal splitter
		m_wndSplitterHorz.m_cxyMin = 100;
		m_wndSplitterHorz.m_cxySplitBar = 4;
		m_wndSplitterHorz.m_bFullDrag = true;
		m_wndSplitterHorz.SetSplitterExtendedStyle(SPLIT_BOTTOMALIGNED | SPLIT_FIXEDBARSIZE, 0);

		if (NULL == m_wndSplitterHorz.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN))
		{
			ATLTRACE("Failed to create horizontal splitter\n");
			return -1;
		}

		CString strTitle;
		// Top
		{
			// Prepare vertical splitter
			m_wndSplitterVert.m_cxyMin = 100;
			m_wndSplitterVert.m_cxySplitBar = 4;
			m_wndSplitterVert.m_bFullDrag = true;
			m_wndSplitterVert.SetSplitterExtendedStyle(SPLIT_FIXEDBARSIZE, 0);

			// Create vertical splitter
			if (NULL == m_wndSplitterVert.Create(m_wndSplitterHorz, rcDefault))
			{
				ATLTRACE("Failed to create vertical splitter\n");
				return -1;
			}

			// Left
			{
				strTitle = L"tree";
				m_wndPaneTree.SetPaneContainerExtendedStyle(PANECNT_FLATBORDER, PANECNT_FLATBORDER);
				if (NULL == m_wndPaneTree.Create(m_wndSplitterVert, strTitle))
				{
					ATLTRACE("Failed to create tree view's pane 'Namespace'\n");
					return -1;
				}
				if (NULL == m_wndItemTree.Create(m_wndPaneTree, rcDefault, strTitle))
				{
					ATLTRACE("Failed to create tree view 'Namespace'\n");
					return -1;
				}
				m_wndPaneTree.SetClient(m_wndItemTree);
			}
			// Right
			{
				strTitle = L"items";
				m_wndPaneList.SetPaneContainerExtendedStyle(PANECNT_FLATBORDER | PANECNT_NOCLOSEBUTTON, PANECNT_FLATBORDER | PANECNT_NOCLOSEBUTTON);
				if (NULL == m_wndPaneList.Create(m_wndSplitterVert, strTitle))
				{
					ATLTRACE("Failed to create list view's pane 'Namespace'\n");
					return -1;
				}
				if (NULL == m_wndItemList.Create(m_wndPaneList, rcDefault, strTitle))
				{
					ATLTRACE("Failed to create list view 'Namespace'\n");
					return -1;
				}
				m_wndPaneList.SetClient(m_wndItemList);
			}

			// Set panes
			m_wndSplitterVert.SetSplitterPanes(m_wndPaneTree, m_wndPaneList);
		}

		// Bottom
		{
			strTitle = L"log";
			m_wndPaneOutput.SetPaneContainerExtendedStyle(PANECNT_FLATBORDER, PANECNT_FLATBORDER);
			if (NULL == m_wndPaneOutput.Create(m_wndSplitterHorz, strTitle))
			{
				ATLTRACE("Failed to create pane 'Output'\n");
				return -1;
			}
			/*	if (NULL == m_wndOutputView.Create(m_wndPaneOutput, rcDefault, strTitle))
			{
			ATLTRACE("Failed to create view 'Output'\n");
			return -1;
			}
			m_wndPaneOutput.SetClient(m_wndOutputView);*/
		}

		// Set panes
		m_wndSplitterHorz.SetSplitterPanes(m_wndSplitterVert, m_wndPaneOutput);
		m_hWndClient = m_wndSplitterHorz.m_hWnd;

		// Set the splitter as the client area window, and resize
		// the splitter to match the frame size.
		UpdateLayout();

	/*	UISetCheck(ID_VIEW_NAMESPACE_TREE, m_wndSplitterVert.GetSinglePaneMode() == SPLIT_PANE_NONE);
		UISetCheck(ID_VIEW_OUTPUT, m_wndSplitterHorz.GetSinglePaneMode() == SPLIT_PANE_NONE);*/


		return 0;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		bHandled = FALSE;
		return 1;
	}

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}
	LRESULT OnMakeTagClasses(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if(GetModel().CollectTagClasses()){
			m_wndItemTree.InitView();
		}
		else{
			AtlMessageBox(NULL, L"Failed to make classes");			
		}
		return 0;
	}
	LRESULT OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		_FileOpen();
		
		return 0;
	}
	LRESULT OnLoadDBM(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		LPCTSTR pszDefExt = _T("mdb");
		CFileDialog dlg(TRUE, pszDefExt, NULL, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST, L"Справочник (*.mdb)\0*.mdb\0");
		INT_PTR nRes = dlg.DoModal();
		ATLASSERT(nRes > 0);
		if (nRes != IDOK)
			return 0;
		Options::SetInFileName(CString(dlg.m_szFileName));		
		if(GetModel().ImportMDB(dlg.m_szFileName)){
			m_wndItemTree.InitView();
			m_wndItemList.SetParentId(ITEM_ID_ROOT);
		}

		return 0;
	}

	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		_FileClose();
		_FileOpen();

		return 0;
	}

	LRESULT OnFileSaveAs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		_FileSave();

		return 0;
	}


	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		static BOOL bVisible = TRUE;	// initially visible
		bVisible = !bVisible;
		CReBarCtrl rebar = m_hWndToolBar;
		int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
		rebar.ShowBand(nBandIndex, bVisible);
		UISetCheck(ID_VIEW_TOOLBAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
		::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
		UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		return 0;
	}
	LRESULT OnItemChanged(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
	{
		//if(hWndCtl == m_wndItemList.m_hWnd){
			_UpdateStatusBar();
		//}
		bHandled = FALSE;
		return 0;
	}


	
};
