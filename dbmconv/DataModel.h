#pragma once
#include <vector>
#include <atldbcli.h>
#include <ATLComTime.h>
#include "item.h"
#include "DeviceInfo.h"
#include "TagInfo.h"
#include "NamespaceDatabaseDictionary.h"

#include "HelpersMDB.h"
#include "HelpersXML.h"

static const int sc_nCurrentVersion = 1;

static const LPCWSTR sc_cwszXmlNodeRoot = L"dictroot";
static const LPCWSTR sc_cwszXmlNodeRootAttrVersion = L"version";
static const LPCWSTR sc_cwszXmlNodeRootAttrDate = L"date";
static const LPCWSTR sc_cwszXmlNodeRootAttrRevision = L"rev";


static const LPCWSTR sc_cwszXmlNodeItem = L"item";
static const LPCWSTR sc_cwszXmlNodeItemAttrType = L"type";
static const LPCWSTR sc_cwszXmlNodeItemAttrID = L"id";
static const LPCWSTR sc_cwszXmlNodeItemAttrName = L"name";
static const LPCWSTR sc_cwszXmlNodeItemAttrDisabled = L"disabled";

static const LPCWSTR sc_cwszXmlNodeItemProperty = L"prop";
static const LPCWSTR sc_cwszXmlNodeItemPropertyAttrID = L"id";
static const LPCWSTR sc_cwszXmlNodeItemPropertyAttrVariantType = L"vt";

typedef std::map<int, IItemPtr> ItemMap;

class CDataModel
{
protected:
	static HRESULT ImportDictionary(LPCTSTR pszFilePath)
	{
		using namespace boost;
		HRESULT hr;
		CDataConnection Connection;
		if (S_OK != (hr = OpenConnection(pszFilePath, false, Connection)))
		{
			AtlMessageBox(NULL, FormatOLEDBMessageSimple().GetBuffer());
			return E_FAIL;
		}

				CCmdDictionaryMeter cmdMeter;
				if (FAILED(hr = cmdMeter.Open(Connection)))
					AtlMessageBox(NULL, FormatOLEDBMessageSimple().GetBuffer());
					//OPCTLTRACE_HL_ET(hr, _T("Ошибка загрузки информации для прибора '%s'. %s"), GetItemFullNameSimple(rspMeter->GetID()), OPCTL::FormatOLEDBMessageSimple());
				else
				{
					int i = 1;
					while (S_OK == (hr = cmdMeter.MoveNext()))
					{
						IItemPtr ptr (new CItemDevice(cmdMeter.m_nID));
						ptr->SetPropertyValue(DevicePropUid, many(cmdMeter.m_nID));
						ptr->SetPropertyValue(DevicePropTypeName, many(wstring(cmdMeter.m_szName)));
						ptr->SetPropertyValue(DevicePropSubClass, many(cmdMeter.m_nSubClass));
						ptr->SetPropertyValue(DevicePropClass, many(cmdMeter.m_nClass));
						ptr->SetPropertyValue(DevicePropResource, many(cmdMeter.m_nResource));

						ptr->SetPropertyValue(DevicePropProtoId, many(cmdMeter.m_nProtocol));
						ptr->SetPropertyValue(DevicePropSelfPower, many(cmdMeter.m_lfSelf));
						m_instance.GetDevs().insert(make_pair(cmdMeter.m_nID, ptr));
					}

					if (FAILED(hr))
					{
						
					}
				}
				std::vector<int> vctAddress;
				vctAddress.reserve(10);
				CCmdDictionaryParam cmdParam;
			//	cmdParam.m_nMeterType = nMeterType;
				if (FAILED(hr = cmdParam.Open(Connection)))
					AtlMessageBox(NULL, FormatOLEDBMessageSimple().GetBuffer());
				else
				{
					while (S_OK == (hr = cmdParam.MoveNext()) && cmdParam.m_nIDStatus != DBSTATUS_S_ISNULL)
					{
						IItemPtr ptr (new CItemTag(cmdParam.m_nID));
						ptr->SetPropertyValue(TagPropName, many(wstring(cmdParam.m_szName)));
						if (cmdParam.m_nIDStatus == DBSTATUS_S_OK)
							ATLVERIFY(ptr->SetPropertyValue(TagPropCode,  many(cmdParam.m_nID)));
						if (cmdParam.m_nTypeStatus == DBSTATUS_S_OK)
							ATLVERIFY(ptr->SetPropertyValue(TagPropType, many(cmdParam.m_nType)));
						if (cmdParam.m_nClassStatus == DBSTATUS_S_OK)
							ATLVERIFY(ptr->SetPropertyValue(TagPropClass, many(cmdParam.m_nClass)));
						if (cmdParam.m_nUnitsTypeStatus == DBSTATUS_S_OK)
							ATLVERIFY(ptr->SetPropertyValue(TagPropUnitsType, many(cmdParam.m_nUnitsType)));
						ATLVERIFY(ptr->SetPropertyValue(TagPropUnitsName, many(wstring(cmdParam.m_szUnitsName))));
						ATLVERIFY(ptr->SetPropertyValue(TagPropCharacter, many(cmdParam.m_nCharacter)));
						ATLVERIFY(ptr->SetPropertyValue(TagPropAssignment, many(cmdParam.m_nAssignment)));
						// Set collector address
						ATLVERIFY(ptr->SetPropertyValue(TagPropAddress,  many(wstring(cmdParam.m_szAddressInCollector))));

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
							ATLVERIFY(ptr->SetPropertyValue(TagPropAddressInMeter, many(strAddress, new atlstr_conv())));
						}
						m_instance.InsertParam(cmdParam.m_nID, ptr);
						
					}

					if (FAILED(hr))
					{
						AtlMessageBox(NULL, FormatOLEDBMessageSimple().GetBuffer());					
					}
				}	
		return S_OK;
	}
	static void BuildAddress(const std::vector<int>& rvctAddress, CString& rstrAddress) {
		CString strNumber;
		rstrAddress.Empty();
		for (std::vector<int>::const_iterator it = rvctAddress.begin(); it != rvctAddress.end(); ++it) {
			if (!rstrAddress.IsEmpty()) {
				// Add separator
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
	CDataModel(){}	
	static CDataModel m_instance;

public:
	static CDataModel& Instance(){
		return m_instance;
	}
public:
	bool GetItems(int nId, ItemMap& mapItems){
		mapItems.clear();
		switch(nId){
			case ItemDevice:
				mapItems = m_mapDevs;
				return true;
			case ItemTag:
				mapItems = m_mapParams;
				return true;
			default: 
				return false;
		}
		return true;
	}
	ItemMap& GetDevs(){
		return m_mapDevs;
	}
	bool InsertParam(int nId, IItemPtr param){
		m_mapParams.insert(make_pair(nId, param));
		return true;
	}
	bool LoadFromMDB(CString strFilePath){
		return SUCCEEDED(ImportDictionary(strFilePath));
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

		//CString strDate;
		//if (FAILED(hr = ToString(CComVariant((DATE)COleDateTime::GetCurrentTime(), VT_DATE), strDate)))
		//	return hr;
		//if (FAILED(hr = Doc.SetAttributeValue(spNodeRoot, sc_cwszXmlNodeRootAttrDate, strDate)))
		//	return hr;

		//OPCTL::CItemList lstItems;
		//ATLVERIFY(GetChildItems(OPCTL::ITEM_ID_ROOT, lstItems));

		/*if (FAILED(hr = SaveItems(Doc, spNodeRoot, lstItems)))
			return hr;
		*/
		if (FAILED(hr = Doc.Save(strFilePath)))
			return hr;

		return S_OK;
	}
protected:
	ItemMap m_mapDevs;
	ItemMap m_mapParams;

};
CDataModel& GetModel();

