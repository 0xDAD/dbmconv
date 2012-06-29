#pragma once

#include <vector>
#include <list>
#include <algorithm>
#include <atldbcli.h>
#include <ATLComTime.h>

#include "ItemFactory.h"
#include "XMLDefs.h"

#include "NamespaceDatabaseDictionary.h"

#include "ContentHandler.h"
#include "HelpersMDB.h"
#include "HelpersXML.h"



class CDataModel
{
protected:
	static HRESULT ImportDictionary(LPCTSTR pszFilePath)
	{
		using namespace boost;
		HRESULT hr;
		CDataConnection Connection;
		vector<int> vctAddress;
		vctAddress.reserve(10);
		if (S_OK != (hr = OpenConnection(pszFilePath, false, Connection)))
		{
			AtlMessageBox(NULL, FormatOLEDBMessageSimple().GetBuffer());
			return E_FAIL;
		}
			CCmdDictionaryMeter cmdMeter;
			if (FAILED(hr = cmdMeter.Open(Connection)))
				AtlMessageBox(NULL, FormatOLEDBMessageSimple().GetBuffer());
			else
			{
				IItemPtr ptrNode;
				m_instance.CreateItem(ItemTypeOldImplNode, ITEM_ID_ROOT, ptrNode);
				ptrNode->SetPropertyValue(BasePropName, many(wstring(L"Old Implementation")));
				int i = 1;
				while (S_OK == (hr = cmdMeter.MoveNext()))
				{
					IItemPtr ptrDev;
					if(m_instance.CreateItem(ItemTypeDevice, ptrNode->GetID(), ptrDev)){
						ptrDev->SetPropertyValue(DevicePropUid, many(cmdMeter.m_nID));
						ptrDev->SetPropertyValue(DevicePropTypeName, many(wstring(cmdMeter.m_szName)));
						ptrDev->SetPropertyValue(DevicePropSubClass, many(cmdMeter.m_nSubClass));
						ptrDev->SetPropertyValue(DevicePropClass, many(cmdMeter.m_nClass));
						ptrDev->SetPropertyValue(DevicePropResource, many(cmdMeter.m_nResource));

						ptrDev->SetPropertyValue(DevicePropProtoId, many(cmdMeter.m_nProtocol));
						ptrDev->SetPropertyValue(DevicePropSelfPower, many(cmdMeter.m_lfSelf));
					}	
						
					CCmdDictionaryParam cmdParam;
					cmdParam.m_nMeterType = cmdMeter.m_nID;
					if (FAILED(hr = cmdParam.Open(Connection)))
						AtlMessageBox(NULL, FormatOLEDBMessageSimple().GetBuffer());
					else
					{
						while (S_OK == (hr = cmdParam.MoveNext()) && cmdParam.m_nIDStatus != DBSTATUS_S_ISNULL)
						{
							IItemPtr ptrParam;
							if(m_instance.CreateItem(ItemTypeTag, ptrDev->GetID(), ptrParam)){
								ptrParam->SetPropertyValue(TagPropName, many(wstring(cmdParam.m_szName)));
								if (cmdParam.m_nIDStatus == DBSTATUS_S_OK)
									ATLVERIFY(ptrParam->SetPropertyValue(TagPropGuid,  many(cmdParam.m_nID)));
								if (cmdParam.m_nTypeStatus == DBSTATUS_S_OK)
									ATLVERIFY(ptrParam->SetPropertyValue(TagPropType, many(cmdParam.m_nType)));
								if (cmdParam.m_nClassStatus == DBSTATUS_S_OK)
									ATLVERIFY(ptrParam->SetPropertyValue(TagPropClass, many(cmdParam.m_nClass)));
								if (cmdParam.m_nUnitsTypeStatus == DBSTATUS_S_OK)
									ATLVERIFY(ptrParam->SetPropertyValue(TagPropUnitsType, many(cmdParam.m_nUnitsType)));
								ATLVERIFY(ptrParam->SetPropertyValue(TagPropUnitsName, many(wstring(cmdParam.m_szUnitsName))));
								ATLVERIFY(ptrParam->SetPropertyValue(TagPropCharacter, many(cmdParam.m_nCharacter)));
								ATLVERIFY(ptrParam->SetPropertyValue(TagPropAssignment, many(cmdParam.m_nAssignment)));
								// Set collector address
								ATLVERIFY(ptrParam->SetPropertyValue(TagPropAddress,  many(wstring(cmdParam.m_szAddressInCollector))));

								CCmdDictionaryAddress cmdAddress;
								cmdAddress.m_nParamID = cmdParam.m_nID;

								HRESULT hrAddress;
								if (FAILED(hrAddress = cmdAddress.Open(Connection))) {
									AtlMessageBox(NULL, FormatOLEDBMessageSimple().GetBuffer());
								}
								else {
									vctAddress.clear();
									while (S_OK == (hrAddress = cmdAddress.MoveNext())) {
										if ((size_t)cmdAddress.m_nPosition >= vctAddress.size())
											vctAddress.resize(cmdAddress.m_nPosition + 1, 0);
										vctAddress[cmdAddress.m_nPosition] = cmdAddress.m_nAddress;
									}
									if (FAILED(hrAddress))
										AtlMessageBox(NULL, FormatOLEDBMessageSimple().GetBuffer());
									cmdAddress.Close();
									CString strAddress;
									BuildAddress(vctAddress, strAddress);
								//	ATLVERIFY(ptrParam->SetPropertyValue(TagPropAddressInMeter, many(strAddress, new atlstr_conv())));
										ATLVERIFY(ptrParam->SetPropertyValue(TagPropAddressInMeter, many(wstring(strAddress.GetBuffer()))));
								}
							}

							if (FAILED(hr))
							{
								AtlMessageBox(NULL, FormatOLEDBMessageSimple().GetBuffer());					
							}
						}
					}	
				}
			}				
		return S_OK;
	}
	static void BuildAddress(const std::vector<int>& rvctAddress, CString& rstrAddress) {
		CString strNumber;
		rstrAddress.Empty();
		for (std::vector<int>::const_iterator it = rvctAddress.begin(); it != rvctAddress.end(); ++it) {
			if (!rstrAddress.IsEmpty()) {
				rstrAddress += _T('.');
			}
			strNumber.Format(_T("%d"), *it);
			rstrAddress += strNumber;
		}
	}
	static HRESULT OpenConnection(LPCTSTR szFilePath, bool bCreateFile, CDataConnection& rConnection)
	{
		CStringW str;
		str.Format(L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;", szFilePath);
		return rConnection.Open(str);
	}
	
private:
	CDataModel():m_nNextId(ITEM_ID_INVALID + 1),
		m_bModified(false)
	{

	}	
	static CDataModel m_instance;

public:
	static CDataModel& Instance(){
		return m_instance;
	}
public:
	bool GetChildItems(int nParentId, int nItemType, ItemList& listItems){
		if(!HasChilds(nParentId))
			return false;
		const ItemList& listChilds = m_treeItems[nParentId];
		for(auto x = listChilds.begin(); x!=listChilds.end();++x){
			const IItemPtr& ptr = *x;
			if(nItemType == ItemTypeNone || ptr->GetType() == nItemType){
				listItems.push_back(ptr);
			}
		}
		return true;
	}
	bool GetChildItemsIDs(int nParentId, int nItemType, std::vector<int>& vctItemIDs){
		if(!HasChilds(nParentId))
			return false;
		const ItemList& listChilds = m_treeItems[nParentId];
		for(auto x = listChilds.begin(); x!=listChilds.end();++x){
			const IItemPtr& ptr = *x;
			if(nItemType == ItemTypeNone || ptr->GetType() == nItemType){
				vctItemIDs.push_back(ptr->GetID());
			}
		}
		return true;
	}
	bool HasChilds( int nParentId ){
		return m_treeItems.find(nParentId) != m_treeItems.end();			
	}
	bool CreateItem(int nType, int nParentId, IItemPtr& rpItem){
		int nId = m_nNextId;
		if(CItemFactory::CreateItem(nType, nId, nParentId, rpItem)){
			m_nNextId++;
			m_mapItems.insert(ItemMap::value_type(nId, rpItem));
			ItemList& brothers = m_treeItems[nParentId];
			brothers.push_back(rpItem);		
			return true;
		}else
			return false;
	}
	bool GetItem(int nId, IItemPtr& rpItem){
		if(!HasItem(nId))
			return false;
		rpItem = m_mapItems[nId];
		return true;
	}
	bool HasItem(int nId){
		return m_mapItems.find(nId) != m_mapItems.end();
	}
	void RebuildItemsTree(){
		m_treeItems.clear();
		int nMaxId = 0;
		for (auto it = m_mapItems.begin(); it != m_mapItems.end(); ++it){
			int nParent = it->second->GetParentID();
			ItemList& brothers = m_treeItems[nParent];
			brothers.push_back(it->second);	
			nMaxId = max(nMaxId, it->second->GetID());
		}
		m_nNextId = nMaxId + 1;	
	}
	bool CollectTagClasses(){
		ItemList ilist;
		if(!m_instance.GetChildItems(ITEM_ID_ROOT, ItemTypeOldImplNode, ilist))
			return false;
		if(!ilist.size() || ilist.size() > 1)
			return false;
		if(!m_instance.GetChildItems(ilist.front()->GetID(), ItemTypeDevice, ilist))
			return false;
		
		map<CString,vector<int>> mapTagClassRef;
		for (auto it = ilist.begin(); it != ilist.end(); ++it){
			ItemList tagList;
			if(!m_instance.GetChildItems((*it)->GetID(), ItemTypeTag, tagList))
				return false;
			for(auto ti = tagList.begin(); ti != tagList.end(); ++ti){
				vector<int>& vctIds = mapTagClassRef[(*ti)->GetName()];
				vctIds.push_back((*ti)->GetID());				
			}
		}

		IItemPtr ptrNewNode;
		ATLVERIFY(m_instance.CreateItem(ItemTypeNewImplNode, ITEM_ID_ROOT, ptrNewNode));			
		ptrNewNode->SetName(L"New Implementation");
		ATLVERIFY(m_instance.CreateItem(ItemTypeTagClassNode, ptrNewNode->GetID(), ptrNewNode));			
		ptrNewNode->SetName(L"Tag Classes");
		int nTagClassNodeId = ptrNewNode->GetID();

		for (auto it = mapTagClassRef.begin(); it != mapTagClassRef.end(); ++it){

			IItemPtr ptr;
			
			if(!m_instance.CreateItem(ItemTypeTagClass, nTagClassNodeId, ptr))
				return false;

			ptr->SetName(it->first);
			_SetItemValue(ptr, TagClassPropRefCnt, it->second.size());			
			
			bool bDifferent = false;
			CString strValue;
			if(GetMultiItemPropertyValueStr(it->second, TagPropType, strValue, bDifferent) && !bDifferent){
				if (!_SetItemValue(ptr, TagClassPropType, wstring(strValue.GetBuffer()))) 
					return false;
			}
			if(GetMultiItemPropertyValueStr(it->second, TagPropAssignment, strValue, bDifferent) && !bDifferent){
				if (!_SetItemValue(ptr, TagClassAssignment,  wstring(strValue.GetBuffer()))) 
					return false;
			}

			for (auto ti = it->second.begin(); ti != it->second.end(); ++ti){
				IItemPtr ptrTag;
				if(GetItem(*ti, ptrTag)){
					_SetItemValue(ptrTag, TagPropRef, ptr->GetID());									
				}
			}
		}
		return true;

	}

protected:

	template <class Type>
	bool _SetItemValue(IItemPtr& ptr, int nPropId, const Type& tval){
		if(ptr->SetPropertyValue(nPropId, many(tval))){
			_SetModified(true);
			return true;
		}
		return false;
	}

	template <class Type>
	bool _SetItemValue(int nID, int nPropId, const Type& tval){		
		IItemPtr ptr;
		if(!GetItem(nID, ptr))
			return false;
		return _SetItemValue(ptr, nPropId, tval);
	}
public:
	bool GetMultiItemPropertyValue(vector<int>& vctIds, int nPropId, many& rValue, bool& rbDifferent){
		rbDifferent = false;
		wstring locVal;
		many val;
		size_t nSucceded = 0;
		for(auto it = vctIds.begin(); it != vctIds.end(); ++it){
			IItemPtr ptrItem;
			if(GetItem(*it, ptrItem)){
				CString strVal;
				if(ptrItem->GetPropertyValue(nPropId, val)){
					if(nSucceded == 0){
						locVal = val.to_string();
						nSucceded++;
					}
					else{
						if(val.to_string() != locVal){
							rbDifferent = true;
							return true;
						}
						nSucceded++;
					}
				}
			}
		}
		if(!nSucceded)
			return false;
		rValue = locVal;
		return true;
	}
	bool GetMultiItemPropertyValueStr(vector<int>& vctIds, int nPropId, CString& rstrValue, bool& rbDifferent){
		rbDifferent = false;
		rstrValue.Empty();
		CString strLocValue;
		size_t nSucceded = 0;
		for(auto it = vctIds.begin(); it != vctIds.end(); ++it){
			IItemPtr ptrItem;
			if(GetItem(*it, ptrItem)){
				CString strVal;
				if(ptrItem->GetPropertyValueText(nPropId, strVal)){
					if(nSucceded == 0){
						strLocValue = strVal;
						nSucceded++;
					}
					else{
						if(strVal != strLocValue){
							rbDifferent = true;
							return true;
						}
						nSucceded++;
					}
				}
			}
		}
		if(!nSucceded)
			return false;
		rstrValue = strLocValue;
		return true;
	}
	bool GetMultiItemType(vector<int>& vctIds, int& rnType){
		int nType = ItemTypeNone, nIType= ItemTypeNone;
		size_t nSucceded = 0;
		for(auto it = vctIds.begin(); it != vctIds.end(); ++it){
			IItemPtr ptrItem;
			if(GetItem(*it, ptrItem)){								
				nIType = ptrItem->GetType();
				if(nSucceded == 0){
					nType = nIType;					
				}
				else
					if(nType != nIType){
						rnType = ItemTypeNone;
						return true;
					}					
				nSucceded++;
			}
		}
		if(!nSucceded)
			return false;
		rnType = nType;
		return true;
	}
public:
	bool ImportMDB(CString strFilePath){
		return SUCCEEDED(ImportDictionary(strFilePath));
	}
	HRESULT LoadFromXML(CString strFilePath){
		ISAXXMLReader* pRdr = NULL;
		HRESULT hr = CoCreateInstance(
			__uuidof(SAXXMLReader60), 
			NULL, 
			CLSCTX_ALL, 
			__uuidof(ISAXXMLReader), 
			(void **)&pRdr);

		if(!FAILED(hr)) 
		{
			CDataModelContentHandler * pMc = new CDataModelContentHandler();
			hr = pRdr->putContentHandler(pMc);
			// Set error handler
			//SAXErrorHandlerImpl * pEc = new SAXErrorHandlerImpl();
			//hr = pRdr->putErrorHandler(pEc);
			hr = pRdr->parseURL(strFilePath);
			if(FAILED(hr)){
				AtlMessageBox(NULL, L"Unable to parse!!!");
			}
			else{
				m_mapItems.swap(pMc->GetItems());			
				RebuildItemsTree();
			}
			pRdr->Release();
			m_bModified = false;
		}
		return hr;

	}
	HRESULT SaveToXML(CString strFilePath){
		HRESULT hr;

		CXMLDOMDocument Doc;
		if (FAILED(hr = Doc.Create()))
			return hr;

		if (FAILED(hr = Doc.AppendProcessingInstruction(L"xml", L"version='1.0' encoding='utf-8'")))
			return hr;

		CComPtr<IXMLDOMElement> spNodeRoot;
		if (FAILED(hr = Doc.AddElementWithEndl(sc_cwszXmlNodeRoot, &spNodeRoot)))
			return hr;

		if (FAILED(hr = Doc.SetAttributeValue(spNodeRoot, sc_cwszXmlNodeRootAttrVersion, CComVariant(sc_nCurrentVersion))))
			return hr;

		if (FAILED(hr = Doc.SetAttributeValue(spNodeRoot, sc_cwszXmlNodeRootAttrRevision, CComVariant(sc_nCurrentVersion))))
			return hr;

		COleDateTime odt = COleDateTime::GetCurrentTime();		
		if (FAILED(hr = Doc.SetAttributeValue(spNodeRoot, sc_cwszXmlNodeRootAttrDate, CComVariant(odt.Format()))))
			return hr;

		ItemList lstItems;
		ATLVERIFY(GetChildItems(ITEM_ID_ROOT, ItemTypeNone, lstItems));

		if (FAILED(hr = SaveItems(Doc, spNodeRoot, lstItems)))
			return hr;

		if (FAILED(hr = Doc.Save(strFilePath)))
			return hr;

		return S_OK;
	}
	HRESULT SaveItems(CXMLDOMDocument& rDoc, IXMLDOMNode* pParent, ItemList& rlstItems){
		HRESULT hr;
		CString strValue;
		for (auto it = rlstItems.begin(); rlstItems.end() != it; ++it)
		{
			const IItemPtr& rspItem = *it;

			CComPtr<IXMLDOMElement> spElement;
			if (FAILED(hr = rDoc.AddElementWithEndl(pParent, sc_cwszXmlNodeItem, &spElement)))
				return hr;
			if (FAILED(hr = rDoc.SetAttributeValue(spElement, sc_cwszXmlNodeItemAttrType, CComVariant(rspItem->GetType()))))
				return hr;
			if (FAILED(hr = rDoc.SetAttributeValue(spElement, sc_cwszXmlNodeItemAttrID, CComVariant(rspItem->GetID()))))
				return hr;
			if (FAILED(hr = rDoc.SetAttributeValue(spElement, sc_cwszXmlNodeItemAttrParID, rspItem->GetParentID())))
				return hr;
			

			// Save custom properties
			CItemPropertyValueMap mapProperties;
			rspItem->GetPropertyValues(mapProperties);
			for (auto it_prop = mapProperties.begin(); it_prop != mapProperties.end(); ++it_prop) {
				// Check for default
				bool bDefault = false;
				many rValue;
				rspItem->GetPropertyValue(it_prop->first, rValue);
				if (FAILED(hr = SaveItemProperty(rDoc, spElement, it_prop->first, rValue.to_string().c_str()))){
					return hr;
				}
			}			
			ItemList lstChildren;
			if (GetChildItems(rspItem->GetID(), ItemTypeNone, lstChildren))
				if (FAILED(hr = SaveItems(rDoc, pParent, lstChildren)))
					return hr;
		}
		return S_OK;
	}
	HRESULT SaveItemProperty(CXMLDOMDocument& rDoc, IXMLDOMNode* pItem, int nPropID, LPCTSTR pszValue)
	{
		HRESULT hr;
		CComPtr<IXMLDOMElement> spElement;
		if (FAILED(hr = rDoc.AddElementWithText(pItem, sc_cwszXmlNodeItemProperty, pszValue, false, &spElement)))
			return hr;
		if (FAILED(hr = rDoc.SetAttributeValue(spElement, sc_cwszXmlNodeItemPropertyAttrID, CComVariant(nPropID))))
			return hr;		
		return S_OK;
	}

private:
	void _SetModified(bool bModified){
		m_bModified = bModified;
	}
	bool _GetModified()
	{
		return m_bModified;
	}

protected:
	ItemMap m_mapItems;
	ItemTree m_treeItems;
private:
	int m_nNextId;
	bool m_bModified;
};

CDataModel& GetModel();

