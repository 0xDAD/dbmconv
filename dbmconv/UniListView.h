#pragma once
#include <atlctrls.h>
#include <atlctrlx.h>
#include "DataModel.h"
#include <boost/lexical_cast.hpp>
using namespace std;
using namespace boost;
typedef ATL::CWinTraits<WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|
	LVS_REPORT | LVS_SHOWSELALWAYS | LVS_EDITLABELS /*| LVS_OWNERDATA*/, WS_EX_CLIENTEDGE> CViewItemListTraits;

//////////////////////////////////////////////////////////////////////////

class CUniListView: public CSortListViewCtrlImpl<CUniListView, CListViewCtrl, CViewItemListTraits>
{
public:
	CUniListView(){		

	}
	BEGIN_MSG_MAP(CUniListView)
		//REFLECTED_NOTIFY_CODE_HANDLER(LVN_GETDISPINFO, OnGetDispInfo)
	END_MSG_MAP()
public:
	void SetCollection();
	void CreateColumns(){		
		
		int nc = 0;
		InsertColumn(nc++, L"id");
		for (int i = DevicePropFirst; i != DevicePropLast; i++)		{
			InsertColumn(nc++, CDeviceProperties::GetPropertyName(i));
			SetColumnWidth(nc - 1 , LVSCW_AUTOSIZE_USEHEADER);
		}		

	}
	void RefreshData(){
		DeleteAllItems();
		ItemMap& imap = GetModel().GetDevs();
		int ni=0;
		for ( auto it = imap.begin(); it != imap.end(); ++it){
			CItemPropertyValueMap props;
			it->second->GetPropertyValues(props);
			int nItem = InsertItem(GetItemCount(), lexical_cast<wstring>(ni++).c_str());
			int nSi = 1;
			for (int i = DevicePropFirst; i != DevicePropLast; i++)		{
				many rValue;
				if(it->second->GetPropertyValue(i, rValue)){
					SetItemText(nItem, nSi++, rValue.to_string().c_str());
				}
			}	
		}

		

	}
protected:
	//LRESULT OnGetDispInfo(int nId, LPNMHDR lpNmhdr, BOOL& bHandled){
	//	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(lpNmhdr);
	//	LV_ITEM* pItem= &(pDispInfo->item);

	//	int iItemIndx= pItem->iItem;

	//	if (pItem->mask & LVIF_TEXT) //valid text buffer?
	//	{
	//		wcscpy(pItem->pszText, L"PREVED");
	//	}		
	//	return 0;
	//}
private:

};