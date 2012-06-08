#pragma once

typedef CWinTraits<WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
	TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_EDITLABELS | TVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE> CUniTreeViewTraits;

//////////////////////////////////////////////////////////////////////////


class CUniTreeView : public CWindowImpl<CUniTreeView, CTreeViewCtrl, CUniTreeViewTraits>
{
	typedef CUniTreeView thisClass;
	typedef CWindowImpl<CUniTreeView, CTreeViewCtrl, CUniTreeViewTraits> baseClass;
	// Direction for sorting
	enum SortDirection
	{
		SortDescending = -1,
		SortNone = 0,
		SortAscending = 1
	};

	// Sort type
	struct SortType
	{
		int nDirection;
		int nPropertyID;
	};

public:
	CUniTreeView()		 {			
	}
	~CUniTreeView() {
		
	}

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		if (pMsg->message == WM_KEYDOWN) {
			// When an item is being edited make sure the edit control
			// receives certain important key strokes
			if (GetEditControl()) {
				if (pMsg->wParam == VK_RETURN ||
					pMsg->wParam == VK_BACK || 
					pMsg->wParam == VK_DELETE ||
					pMsg->wParam == VK_ESCAPE ||
					GetKeyState(VK_CONTROL))
				{
					::TranslateMessage(pMsg);
					::DispatchMessage(pMsg);
					return TRUE;
				}
			}
		}
		return FALSE;
	}
protected:
	BEGIN_MSG_MAP(thisClass)
	END_MSG_MAP()
};
