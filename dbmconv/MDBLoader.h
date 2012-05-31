#pragma once
#include <atldbcli.h>
#include <vector>
#include "NamespaceDatabaseDictionary.h"
#include "DataModel.h"


inline HRESULT FormatSystemMessage(HRESULT hr, CString& rstrMessage, LCID lcLocale = LOCALE_SYSTEM_DEFAULT, ...)
{
	va_list argList;
	va_start(argList, lcLocale);

	HRESULT hrRes;
	LPTSTR pszMessage = NULL;
	if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, LANGIDFROMLCID(lcLocale), (LPTSTR)&pszMessage, 0, &argList)) {
		rstrMessage = pszMessage;
		::LocalFree(pszMessage);
		hrRes = S_OK;
	}
	else {
		hrRes = AtlHresultFromLastError();
	}

	va_end(argList);

	return hrRes;
}
inline CString FormatSystemMessageSimple(HRESULT hr, LCID lcLocale = LOCALE_SYSTEM_DEFAULT)
{
	CString strMsg;
	FormatSystemMessage(hr, strMsg, lcLocale);
	return strMsg;
}
inline CString FormatOLEDBMessageSimple(LCID lcLocale = LOCALE_SYSTEM_DEFAULT)
{
	CString strMessage;

	HRESULT hr = S_OK;
	ULONG cRecords = 0;
	CDBErrorInfo ErrorInfo;
	if (FAILED(hr = ErrorInfo.GetErrorRecords(&cRecords)) || ErrorInfo.m_spErrorInfo == NULL)
		return strMessage;

	CComBSTR bstrDesc, bstrSource;

	ATLASSERT(ErrorInfo.m_spErrorInfo);

	// Convert locate
	lcLocale = ::ConvertDefaultLocale(lcLocale);

	for (ULONG i = 0; i < cRecords; i++)
	{
		hr = ErrorInfo.GetAllErrorInfo(i, lcLocale, &bstrDesc, &bstrSource);
		if (FAILED(hr))
			return FormatSystemMessageSimple(hr, lcLocale);
		strMessage += bstrDesc;
		bstrSource.Empty();
		bstrDesc.Empty();
	}
	return strMessage;
}


class CNamespace
{
public:
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
						ptr->SetPropertyValue(DevicePropUid, any(cmdMeter.m_nID));
						ptr->SetPropertyValue(DevicePropTypeName, any(CString(cmdMeter.m_szName)));
						ptr->SetPropertyValue(DevicePropSubClass, any(cmdMeter.m_nSubClass));
						ptr->SetPropertyValue(DevicePropClass, any(cmdMeter.m_nClass));
						ptr->SetPropertyValue(DevicePropResource, any(cmdMeter.m_nResource));

						ptr->SetPropertyValue(DevicePropProtoId, any(cmdMeter.m_nProtocol));
						ptr->SetPropertyValue(DevicePropSelfPower, any(cmdMeter.m_lfSelf));
						GetModel().GetDevs().insert(make_pair(cmdMeter.m_nID, ptr));
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
						ptr->SetPropertyValue(TagPropName, any(CString(cmdParam.m_szName)));
						if (cmdParam.m_nIDStatus == DBSTATUS_S_OK)
							ATLVERIFY(ptr->SetPropertyValue(TagPropCode,  any(cmdParam.m_nID)));
						if (cmdParam.m_nTypeStatus == DBSTATUS_S_OK)
							ATLVERIFY(ptr->SetPropertyValue(TagPropType, any(cmdParam.m_nType)));
						if (cmdParam.m_nClassStatus == DBSTATUS_S_OK)
							ATLVERIFY(ptr->SetPropertyValue(TagPropClass, any(cmdParam.m_nClass)));
						if (cmdParam.m_nUnitsTypeStatus == DBSTATUS_S_OK)
							ATLVERIFY(ptr->SetPropertyValue(TagPropUnitsType, any(cmdParam.m_nUnitsType)));
						ATLVERIFY(ptr->SetPropertyValue(TagPropUnitsName, any(CString(cmdParam.m_szUnitsName))));
						ATLVERIFY(ptr->SetPropertyValue(TagPropCharacter, any(cmdParam.m_nCharacter)));
						ATLVERIFY(ptr->SetPropertyValue(TagPropAssignment, any(cmdParam.m_nAssignment)));
						// Set collector address
						ATLVERIFY(ptr->SetPropertyValue(TagPropAddress,  any(CString(cmdParam.m_szAddressInCollector))));

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
							ATLVERIFY(ptr->SetPropertyValue(TagPropAddressInMeter, any(strAddress)));
						}
						GetModel().InsertParam(cmdParam.m_nID, ptr);
						
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
	/*	HRESULT hr;
		if (bCreateFile)
		{
			if (FAILED(hr = Helpers::AtlSaveResourceToFile(szFilePath, IDR_DATABASE, _T("DATABASE"))))
				return hr;
		}*/

		CStringW str;
		//	str.Format(L"Driver={Microsoft Access Driver (*.mdb)};DBQ=%s;", szFilePath);
		str.Format(L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;", szFilePath);
		return rConnection.Open(str);
	}

protected:
private:
};