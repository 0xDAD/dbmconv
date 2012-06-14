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
	void InitView(){
		DeleteAllItems();		
		HTREEITEM hParent = InsertItem(TVIF_TEXT | TVIF_PARAM,
			L"Root", 0, 0, 0, 0, static_cast<LPARAM>(ITEM_ID_ROOT), TVI_ROOT, TVI_LAST);
		
		_InsertItemWithChildren(hParent, ITEM_ID_ROOT);
		Expand(hParent, TVE_EXPAND);
	}
	HTREEITEM _InsertItemWithChildren(HTREEITEM hParent, int nItemID){
		ATLASSERT(hParent);
		if (hParent == NULL)
			return NULL;
		IItemPtr spItem;
		if (GetModel().GetItem(nItemID, spItem)) {
			//int nImage = TResourceHolder::GetItemImageIndex(spItem->GetType(), spItem->IsDisabled(), spItem);
			hParent = InsertItem(TVIF_TEXT | /*TVIF_IMAGE | TVIF_SELECTEDIMAGE |*/ TVIF_PARAM,
				spItem->GetName(), 0, 0, 0, 0, static_cast<LPARAM>(nItemID), hParent, TVI_LAST);
			if (hParent == NULL)
				return NULL;
		}
		std::vector<int> vctItemIDs;
		if (GetModel().GetChildItemsIDs(nItemID, ItemTypeNone, vctItemIDs)) {
			
			for (auto it = vctItemIDs.begin(); vctItemIDs.end() != it; ++it)
				_InsertItemWithChildren(hParent, *it);
		}
		return hParent;
	}
	void _ExpandItem(HTREEITEM hParent) {
		if (ItemHasChildren(hParent)) {
			HTREEITEM hNextItem;
			HTREEITEM hChildItem = GetChildItem(hParent);
			
			while (hChildItem != NULL) {
				_ExpandItem(hChildItem);
				hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
				hChildItem = hNextItem;
			}
			Expand(hParent, TVE_EXPAND);
		}
	}
	void _CollapseItem(HTREEITEM hParent) {
		if (ItemHasChildren(hParent)) {
			Expand(hParent, TVE_COLLAPSE);
			HTREEITEM hNextItem;
			HTREEITEM hChildItem = GetChildItem(hParent);
			
			while (hChildItem != NULL) {
				_CollapseItem(hChildItem);
				hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
				hChildItem = hNextItem;
			}
		}
	}
protected:
	BEGIN_MSG_MAP(thisClass)
		REFLECTED_NOTIFY_CODE_HANDLER(TVN_SELCHANGED, OnSelChanged)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()
protected:
	LRESULT OnSelChanged(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		NMTREEVIEW* pNMTV = (NMTREEVIEW*)pnmh;
		int nItemID = (int)pNMTV->itemNew.lParam;				
		::SendMessage(GetTopLevelWindow(), WM_MFRAME_NOTIFY, UpdateSelection, nItemID);
		return 0;
	}
};
