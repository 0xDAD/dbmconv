//////////////////////////////////////////////////////////////////////
//
// WTL-based classes for save window settings in registry.
//
// Written by Yury Bura (splin@tut.by)
// Copyright© 2003 Yury Bura.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
//////////////////////////////////////////////////////////////////////

#ifndef __WTLREGPOS_H__
#define __WTLREGPOS_H__

#pragma once

#ifndef __cplusplus
	#error wtlregpos.h requires C++ compilation (use a .cpp suffix)
#endif

#include <atlmisc.h>
#include <atlctrls.h>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CWindowSettings
// CReBarSettings
// CSplitterSettings

//////////////////////////////////////////////////////////////////////
// class CWindowSettings

static const TCHAR s_szWindowPlacement[] = _T("WindowPlacement");

class CWindowSettings
{
public:
	WINDOWPLACEMENT m_WindowPlacement;

	CWindowSettings()
	{
		m_WindowPlacement.length = sizeof(m_WindowPlacement);
		m_WindowPlacement.flags = 0;
		m_WindowPlacement.ptMinPosition.x = 0;
		m_WindowPlacement.ptMinPosition.y = 0;
		m_WindowPlacement.ptMaxPosition.x = 0;
		m_WindowPlacement.ptMaxPosition.y = 0;
		
		CRect rc;
		SystemParametersInfo(SPI_GETWORKAREA, 0, rc, 0);
		rc.DeflateRect(100,100);
		m_WindowPlacement.rcNormalPosition = rc;
		m_WindowPlacement.showCmd = SW_SHOWNORMAL;
	}

	void GetFrom(CWindow& Wnd)
	{
		ATLASSERT(Wnd.IsWindow());
		Wnd.GetWindowPlacement(&m_WindowPlacement);
	}

	void ApplyTo(CWindow& Wnd, int nCmdShow = SW_SHOWNORMAL) const
	{
		ATLASSERT(Wnd.IsWindow());
		if (Wnd.SetWindowPlacement(&m_WindowPlacement))
		{
			if (SW_SHOWNORMAL != nCmdShow)	
				Wnd.ShowWindow(nCmdShow);		
			else if (m_WindowPlacement.showCmd == SW_MINIMIZE || m_WindowPlacement.showCmd == SW_SHOWMINIMIZED)
				Wnd.ShowWindow(SW_SHOWNORMAL);
		}
	}

	bool Load(LPCTSTR szRegKey, LPCTSTR szPrefix, HKEY hkRootKey = HKEY_CURRENT_USER)
	{
		CRegKey RegKey;
		LONG lRes = RegKey.Open(hkRootKey, szRegKey, KEY_READ);
		if (lRes == ERROR_SUCCESS)
		{
			DWORD dwType = NULL;
			DWORD dwSize = sizeof(m_WindowPlacement);
			lRes = RegQueryValueEx(RegKey.m_hKey, CString(szPrefix) + s_szWindowPlacement, NULL, &dwType,
				(LPBYTE)&m_WindowPlacement, &dwSize);
		}
		return lRes == ERROR_SUCCESS;
	}

	bool Save(LPCTSTR szRegKey, LPCTSTR szPrefix, HKEY hkRootKey = HKEY_CURRENT_USER) const
	{
		CRegKey RegKey;
		DWORD lRes = RegKey.Create(hkRootKey, szRegKey);
		if (lRes == ERROR_SUCCESS)
		{
			lRes = RegSetValueEx(RegKey.m_hKey, CString(szPrefix) + s_szWindowPlacement, NULL, REG_BINARY,
				(LPBYTE)&m_WindowPlacement, sizeof(m_WindowPlacement));
		}
		return lRes == ERROR_SUCCESS;
	}
};

//////////////////////////////////////////////////////////////////////
// class CReBarSettings

static const TCHAR s_szReBarBandCount[] = _T("BandCount");
static const TCHAR s_szReBarID[] = _T("ID");
static const TCHAR s_szReBarCX[] = _T("CX");
static const TCHAR s_szReBarBreakLine[] = _T("BreakLine");

class CReBarSettings
{
public:
	struct BandInfo
	{
		DWORD dwID;
		DWORD dwCX;
		bool bBreakLine;
	}* m_pBands;

	DWORD m_dwBandCount;

// Constructors
public:
	CReBarSettings()
	{
		m_pBands = NULL;
		m_dwBandCount = 0;
	}
	
	~CReBarSettings()
	{
		if(m_pBands != NULL)
			delete[] m_pBands;
	}

	void GetFrom(CReBarCtrl& ReBar)
	{
		ATLASSERT(ReBar.IsWindow());
		if(m_pBands != NULL)
			delete[] m_pBands;
		m_pBands = NULL;
		m_dwBandCount = ReBar.GetBandCount();
		if(m_dwBandCount > 0)
			m_pBands = new BandInfo[m_dwBandCount];
		for (UINT i = 0; i < m_dwBandCount; i++)
		{
			REBARBANDINFO rbi;
			rbi.cbSize = sizeof(rbi);
			rbi.fMask = RBBIM_ID | RBBIM_SIZE | RBBIM_STYLE;
			if (ReBar.GetBandInfo(i, &rbi))
			{
				m_pBands[i].dwID = rbi.wID;
				m_pBands[i].dwCX = rbi.cx;
				m_pBands[i].bBreakLine = (rbi.fStyle & RBBS_BREAK) != 0;
			}
		}
	}

	void ApplyTo(CReBarCtrl& ReBar) const
	{
		ATLASSERT(ReBar.IsWindow());
		for (DWORD i = 0; i < m_dwBandCount; i++)
		{
			ReBar.MoveBand(ReBar.IdToIndex(m_pBands[i].dwID), i);
			REBARBANDINFO rbi;
			rbi.cbSize = sizeof(rbi);
			rbi.fMask = RBBIM_ID | RBBIM_SIZE | RBBIM_STYLE;
			ReBar.GetBandInfo(i, &rbi);
			rbi.cx = m_pBands[i].dwCX;
			if(m_pBands[i].bBreakLine)
				rbi.fStyle |= RBBS_BREAK;
			else
				rbi.fStyle &= ~RBBS_BREAK;
			ReBar.SetBandInfo(i, &rbi);
		}
	}

	bool Load(LPCTSTR szRegKey, LPCTSTR szPrefix, HKEY hkRootKey = HKEY_CURRENT_USER)
	{
		if(m_pBands != NULL)
			delete[] m_pBands;
		m_pBands = NULL;
		m_dwBandCount = 0;
		CRegKey RegKey;
		LONG lRes = RegKey.Open(hkRootKey, szRegKey, KEY_READ);
		if (lRes == ERROR_SUCCESS)
		{
#if (_ATL_VER >= 0x0700)
			lRes = RegKey.QueryDWORDValue(CString(szPrefix) + s_szReBarBandCount, m_dwBandCount);
#else
			lRes = RegKey.QueryValue(m_dwBandCount, CString(szPrefix) + s_szReBarBandCount);
#endif //(_ATL_VER >= 0x0700)
			if (lRes == ERROR_SUCCESS)
			{
				if(m_dwBandCount > 0)
					m_pBands = new BandInfo[m_dwBandCount];
				for (UINT i = 0; i < m_dwBandCount; i++)
				{
					CString s;
					s.Format(_T("%s%i_"), szPrefix, i);
#if (_ATL_VER >= 0x0700)
					RegKey.QueryDWORDValue(s + s_szReBarID, m_pBands[i].dwID);
					RegKey.QueryDWORDValue(s + s_szReBarCX, m_pBands[i].dwCX);
#else
					RegKey.QueryValue(m_pBands[i].dwID, s + s_szReBarID);
					RegKey.QueryValue(m_pBands[i].dwCX, s + s_szReBarCX);
#endif //(_ATL_VER >= 0x0700)

					DWORD dw;
#if (_ATL_VER >= 0x0700)
					RegKey.QueryDWORDValue(s + s_szReBarBreakLine, dw);
#else
					RegKey.QueryValue(dw, s + s_szReBarBreakLine);
#endif //(_ATL_VER >= 0x0700)
					m_pBands[i].bBreakLine = (dw != 0);
				}
			}
		}
		return lRes == ERROR_SUCCESS;
	}
	
	bool Save(LPCTSTR szRegKey, LPCTSTR szPrefix, HKEY hkRootKey = HKEY_CURRENT_USER) const
	{
		CRegKey RegKey;
		LONG lRes = RegKey.Create(hkRootKey, szRegKey);
		if (lRes == ERROR_SUCCESS)
		{
#if (_ATL_VER >= 0x0700)
			lRes = RegKey.SetDWORDValue(CString(szPrefix)+ s_szReBarBandCount, m_dwBandCount);
#else
			lRes = RegKey.SetValue(m_dwBandCount, CString(szPrefix)+ s_szReBarBandCount);
#endif //(_ATL_VER >= 0x0700)
			for (DWORD i = 0; i < m_dwBandCount; i++)
			{
				CString s;
				s.Format(_T("%s%i_"), szPrefix, i);
#if (_ATL_VER >= 0x0700)
				RegKey.SetDWORDValue(s + s_szReBarID, m_pBands[i].dwID);
				RegKey.SetDWORDValue(s + s_szReBarCX, m_pBands[i].dwCX);
#else
				RegKey.SetValue(m_pBands[i].dwID, s + s_szReBarID);
				RegKey.SetValue(m_pBands[i].dwCX, s + s_szReBarCX);
#endif //(_ATL_VER >= 0x0700)

				DWORD dw = m_pBands[i].bBreakLine;
#if (_ATL_VER >= 0x0700)
				RegKey.SetDWORDValue(s + s_szReBarBreakLine, dw);
#else
				RegKey.SetValue(dw, s + s_szReBarBreakLine);
#endif //(_ATL_VER >= 0x0700)
			}
		}
		return lRes == ERROR_SUCCESS;
	}
};

//////////////////////////////////////////////////////////////////////
// class CSplitterSettings
//////////////////////////////////////////////////////////////////////

static const TCHAR s_szSplitterPos[] = _T("SplitterPos");
static const TCHAR s_szSplitterSinglePaneMode[] = _T("SplitterSinglePaneMode");

class CSplitterSettings
{
public:
	CSplitterSettings() : m_dwPos((DWORD)-1), m_dwSinglePaneMode((DWORD)-1)
	{
	}

	DWORD m_dwPos;
	DWORD m_dwSinglePaneMode;

	template <class T>
	void GetFrom(const T& Splitter)
	{
		m_dwPos = Splitter.GetSplitterPos();
		m_dwSinglePaneMode = Splitter.GetSinglePaneMode();
	}

	template <class T>
	void ApplyTo(T& Splitter, bool bUpdate = true) const
	{
		Splitter.SetSplitterPos(m_dwPos, bUpdate);
		Splitter.SetSinglePaneMode(m_dwSinglePaneMode);
	}

	bool Load(LPCTSTR lpszRegKey, LPCTSTR lpszPrefix, HKEY hkRootKey = HKEY_CURRENT_USER)
	{
		ATLASSERT(lpszRegKey);
		ATLASSERT(lpszPrefix);
		CRegKey RegKey;
		LONG lRes = RegKey.Open(hkRootKey, lpszRegKey, KEY_READ);
		if (lRes == ERROR_SUCCESS)
			RegKey.QueryDWORDValue(CString(lpszPrefix) + s_szSplitterPos, m_dwPos);
		if (lRes == ERROR_SUCCESS)
			RegKey.QueryDWORDValue(CString(lpszPrefix) + s_szSplitterSinglePaneMode, m_dwSinglePaneMode);
		return lRes == ERROR_SUCCESS;
	}

	bool Save(LPCTSTR lpszRegKey, LPCTSTR lpszPrefix, HKEY hkRootKey = HKEY_CURRENT_USER) const
	{
		ATLASSERT(lpszRegKey);
		ATLASSERT(lpszPrefix);
		if (NULL == lpszRegKey || NULL == lpszPrefix)
			return false;
		CRegKey RegKey;
		LONG lRes = RegKey.Create(hkRootKey, lpszRegKey);
		if (lRes == ERROR_SUCCESS)
			RegKey.SetDWORDValue(CString(lpszPrefix) + s_szSplitterPos, m_dwPos);
		if (lRes == ERROR_SUCCESS)
			RegKey.SetDWORDValue(CString(lpszPrefix) + s_szSplitterSinglePaneMode, m_dwSinglePaneMode);
		return lRes == ERROR_SUCCESS;
	}
};

#endif // !__WTLREGPOS_H__