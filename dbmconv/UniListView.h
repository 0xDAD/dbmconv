#pragma once
#include <atlctrls.h>
#include <atlctrlx.h>
#include "DataModel.h"
#include <boost/lexical_cast.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/shared_ptr.hpp>
#include <algorithm>

using namespace std;
using namespace boost;

typedef ATL::CWinTraits<WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
	LVS_REPORT | LVS_SHOWSELALWAYS | LVS_EDITLABELS, WS_EX_CLIENTEDGE> CViewItemListTraits;

//////////////////////////////////////////////////////////////////////////

class ILVDataManager{
public:
	virtual bool GetColumns(std::vector<CString>& vctCols) = 0;
	virtual bool GetItems(ItemMap& mapItems) = 0;
	virtual int GetMinPropId() = 0;
	virtual int GetMaxPropId() = 0;

};

template <class TItemClass, class TResourceHolder>
class ILVDataManagerImpl: public ILVDataManager
{
public:
	ILVDataManagerImpl(){}
	virtual ~ILVDataManagerImpl(){}
public:
	bool GetColumns(std::vector<CString>& vctCols)
	{
		vctCols.clear();
		for (int i = TResourceHolder::GetMinPropId(); i <= TResourceHolder::GetMaxPropId(); i++){
			vctCols.push_back(TResourceHolder::GetPropertyName(i));
		}	
		return vctCols.size() > 0;
	}
	bool GetItems(ItemMap& mapItems){
		return GetModel().GetItems(TItemClass::GetTypeID_static(), mapItems);
	}
	int GetMinPropId(){
		return TResourceHolder::GetMinPropId();
	}
	int GetMaxPropId(){
		return TResourceHolder::GetMaxPropId();
	}
};

class CUniListView: public CSortListViewCtrlImpl<CUniListView, CListViewCtrl, CViewItemListTraits>
{
	typedef boost::shared_ptr<ILVDataManager> ILVDataManagerPtr;
public:
	CUniListView(){		

	}
	BEGIN_MSG_MAP(CUniListView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
	END_MSG_MAP()
public:
	void SetCollection(ILVDataManager* fptr){
		m_dm = ILVDataManagerPtr(fptr);
		DeleteAllItems();
		DeleteAllColumns();
		CreateColumns();
		RefreshData();
	}
	void DeleteAllColumns(){
		int nCols = GetHeader().GetItemCount() - 1;
		while(nCols >=0 ){
			GetHeader().DeleteItem(nCols--);
		}
	}
	void CreateColumns(){		
		using namespace boost::lambda;

		if(!m_dm)
			return;
		vector<CString> vctCols;
		if(m_dm->GetColumns(vctCols)){
			int n = 0;
			for(auto x = vctCols.begin(); x != vctCols.end(); ++x){
				InsertColumn(n++, *x);
			}
		}		
		AdjustColumnWidths();
	}
	void AdjustColumnWidths(){
		int nCols = GetHeader().GetItemCount();
		for(int i = 0; i < nCols; i++){
			SetColumnWidth(i , LVSCW_AUTOSIZE_USEHEADER);
		}
	}
	void RefreshData(){
		if(!m_dm)
			return;
		DeleteAllItems();

		ItemMap imap;
		if(m_dm->GetItems(imap)){
			int ni=0;
			for ( auto it = imap.begin(); it != imap.end(); ++it){
				CItemPropertyValueMap props;
				it->second->GetPropertyValues(props);
				int nItem = InsertItem(GetItemCount(), lexical_cast<wstring>(ni++).c_str());			
				int nSi = 1;
				for (int i = m_dm->GetMinPropId(); i <= m_dm->GetMaxPropId(); i++)		{
					many rValue;
					if(it->second->GetPropertyValue(i, rValue)){
						SetItemText(nItem, nSi++, rValue.to_string().c_str());
					}
				}	
			}
		}
		
		AdjustColumnWidths();

	}

protected:
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		LRESULT lResult = DefWindowProc(uMsg, wParam, lParam);
		SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		return lResult;
	}
private:
	ILVDataManagerPtr m_dm;
};