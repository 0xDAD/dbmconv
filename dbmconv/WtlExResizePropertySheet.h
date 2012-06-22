//////////////////////////////////////////////////////////////////////////
//
// WTL-based classes for resize property sheet.
//
// Written by Yury Bura (splin@tut.by)
// Copyright© 2003-2008 Yury Bura.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
//////////////////////////////////////////////////////////////////////////
// Revision 1.1
// Date 2008/09/23
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __ATLBASE_H__
	#error This header requires atlbase.h to be included first
#endif

#ifndef __ATLDLGS_H__
	#error This header requires atldlgs.h to be included first
#endif

//////////////////////////////////////////////////////////////////////////

#include "WtlExWindowSettings.h"

//////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CPropertySheetResize

namespace WTLEX
{

//////////////////////////////////////////////////////////////////////////
// CPropertySheetResize - resize property page

class CPropertySheetResize : public CPropertySheetImpl<CPropertySheetResize>,
	public CDialogResize<CPropertySheetResize>
{
	typedef CPropertySheetImpl<CPropertySheetResize> baseClass;
	typedef CDialogResize<CPropertySheetResize> baseResize;

// Construction
public:
	CPropertySheetResize(_U_STRINGorID title = (LPCTSTR)NULL, UINT uStartPage = 0, HWND hWndParent = NULL)
		: baseClass(title, uStartPage, hWndParent), m_bInitialized(false), m_nLastActivePageIndex(0)
	{
	}

	static int CALLBACK PropSheetCallback(HWND hWnd, UINT uMsg, LPARAM lParam)
	{
		// Dialog template is available and changeable pre-creation
		if (uMsg == PSCB_PRECREATE)
		{
			LPDLGTEMPLATE lpDT = (LPDLGTEMPLATE)lParam;
			// Remove dialog border styles
			lpDT->style &= ~DS_MODALFRAME;
			// Add child window and clipping styles
			lpDT->style |= WS_THICKFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		}
		return baseClass::PropSheetCallback(hWnd, uMsg, lParam);
	}

// Attributes
public:
	int GetLastActivePageIndex() const {
		return m_nLastActivePageIndex;
	}
	void SetRegSettings(LPCTSTR lpszKey, LPCTSTR lpszPrefix) {
		m_strRegKey = lpszKey;
		m_strRegPrefix = lpszPrefix;
	}

// Message map and handlers
public:
	BEGIN_MSG_MAP(CPropertySheetResize)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		if (m_bInitialized)
		{
			CHAIN_MSG_MAP(CDialogResize<CPropertySheetResize>)
		}
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

	BEGIN_DLGRESIZE_MAP(CPropertySheetResize)
		DLGRESIZE_CONTROL(ATL_IDC_TAB_CONTROL, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDOK, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(ID_APPLY_NOW, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDHELP, DLSZ_MOVE_X | DLSZ_MOVE_Y)
	END_DLGRESIZE_MAP()
	
	LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		// If wParam is TRUE, the window is being shown.
		// If wParam is FALSE, the window is being hidden.
		if (wParam == TRUE)
		{
			// Only do this once
			if (!m_bInitialized)
			{
				m_bInitialized = true;
				OnInitialize();
			}
		}
		// Ensure base-class gets this message
		bHandled = FALSE;
		return 0;
	}
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		m_nLastActivePageIndex = GetActiveIndex();
		m_bInitialized = false;
		SaveWindowSettings();
		// Ensure base-class gets this message
		bHandled = FALSE;
		return 0;
	}

// Implementation
protected:
	void OnInitialize()
	{
		DlgResize_Init();

		// Add active page to the resize map
		CWindow wndPage = GetActivePage();
		ATLASSERT(wndPage.IsWindow());
		if (wndPage.IsWindow())
		{
			RECT rectCtl = { 0 };
			wndPage.GetWindowRect(&rectCtl);
			::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rectCtl, 2);
			_AtlDlgResizeData data = { wndPage.GetDlgCtrlID(), DLSZ_SIZE_X | DLSZ_SIZE_Y, { rectCtl.left, rectCtl.top, rectCtl.right, rectCtl.bottom } };
			m_arrData.Add(data);
		}

		LoadWindowSettings();
	}
	bool SaveWindowSettings()
	{
		if (m_strRegKey.IsEmpty() || m_strRegPrefix.IsEmpty())
			return false;
		CWindowSettings WindowSettings;
		WindowSettings.GetFrom(*this);
		return WindowSettings.Save(m_strRegKey, m_strRegPrefix);
	}
	bool LoadWindowSettings()
	{
		if (m_strRegKey.IsEmpty() || m_strRegPrefix.IsEmpty())
			return false;
		CWindowSettings WindowSettings;
		if (!WindowSettings.Load(m_strRegKey, m_strRegPrefix))
			return false;
		WindowSettings.ApplyTo(*this);
		return true;
	}
protected:
	bool m_bInitialized;
	int m_nLastActivePageIndex;
	CString m_strRegKey;
	CString m_strRegPrefix;
};

}; // namespace WTLEX

//////////////////////////////////////////////////////////////////////////
