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
	virtual bool GetItems(ItemList& listItems) = 0;
	virtual int GetMinPropId() = 0;
	virtual int GetMaxPropId() = 0;

};
typedef boost::shared_ptr<ILVDataManager> ILVDataManagerPtr;
template <class TItemClass, class TResourceHolder>
class ILVDataManagerImpl: public ILVDataManager
{
public:
	ILVDataManagerImpl(int nParentId):m_nParent(nParentId){}
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
	bool GetItems(ItemList& listItems){
		return GetModel().GetChildItems(m_nParent, ItemTypeNone, listItems);
	}
	int GetMinPropId(){
		return TResourceHolder::GetMinPropId();
	}
	int GetMaxPropId(){
		return TResourceHolder::GetMaxPropId();
	}

protected:
	int m_nParent;
};
class DataManagerFactory
{
public:
	bool CreateDataManager(int nParent, ILVDataManagerPtr& ptr){
		if(!GetModel().HasChilds(nParent))
			return false;
		if(nParent == ITEM_ID_ROOT){
			ptr = ILVDataManagerPtr(new ILVDataManagerImpl<CNewImplNode, CEmptyProperties>(nParent));
			return true;
		}
		IItemPtr pParent;		
		if(GetModel().GetItem(nParent, pParent)){
			switch(pParent->GetType()){
			case OldImplNode:			
				ptr = ILVDataManagerPtr(new ILVDataManagerImpl<CItemDevice, CDeviceProperties>(nParent));
				return true;
			case ItemDevice:
				ptr = ILVDataManagerPtr(new ILVDataManagerImpl<CItemTag, CTagProperties>(nParent));
				return true;
			default:
				return false;
			}
		}
		return false;
	}
};

class CUniListView: public CSortListViewCtrlImpl<CUniListView, CListViewCtrl, CViewItemListTraits>
{
public:
	DECLARE_WND_CLASS(L"UniListView");
public:
	CUniListView():m_nParentId(ITEM_ID_INVALID){		

	}
	BEGIN_MSG_MAP(CUniListView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()
public:
	void DeleteAllColumns(){
		int nCols = GetHeader().GetItemCount() - 1;
		while(nCols >=0 ){
			GetHeader().DeleteItem(nCols--);
		}
	}
	void SetParentId(int nId)
	{
		if(nId && m_nParentId != nId && GetModel().HasChilds(nId)){
			m_nParentId = nId;
			if(m_dmFact.CreateDataManager(m_nParentId, m_dm)){
				CreateColumns();
				RefreshData();
				RedrawWindow();
			}
		}			
	}
	void CreateColumns(){	
		DeleteAllColumns();
		using namespace boost::lambda;
		if(!m_dm)
			return;
		InsertColumn(0, L"ID");
		vector<CString> vctCols;
		if(m_dm->GetColumns(vctCols)){
			int n = 1;
			for(auto x = vctCols.begin(); x != vctCols.end(); ++x){
				InsertColumn(n++, *x);
			}
		}		
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

		ItemList items;
		if(m_dm->GetItems(items)){
			int ni=0;
			for ( auto it = items.begin(); it != items.end(); ++it){

				int nItem = InsertItem(GetItemCount(), (boost::lexical_cast<wstring>((*it)->GetID())).c_str());			
				int nSi = 1;
				for (int i = m_dm->GetMinPropId(); i <= m_dm->GetMaxPropId(); i++)		{
					many rValue;
					if((*it)->GetPropertyValue(i, rValue)){
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
	DataManagerFactory m_dmFact;
	ILVDataManagerPtr m_dm;
	int m_nParentId;
};