#pragma once
#include <atldbcli.h>
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
						IItemPtr ptr (new CItemDevice(i++));
						ptr->SetPropertyValue(DevicePropUid, any(cmdMeter.m_nID));
						ptr->SetPropertyValue(DevicePropTypeName, any(CString(cmdMeter.m_szName)));
						ptr->SetPropertyValue(DevicePropSubClass, any(cmdMeter.m_nSubClass));
						ptr->SetPropertyValue(DevicePropClass, any(cmdMeter.m_nClass));
						ptr->SetPropertyValue(DevicePropResource, any(cmdMeter.m_nResource));

						ptr->SetPropertyValue(DevicePropProtoId, any(cmdMeter.m_nProtocol));
						ptr->SetPropertyValue(DevicePropSelfPower, any(cmdMeter.m_lfSelf));
						GetModel().GetDevs().insert(make_pair(i, ptr));
					}

					if (FAILED(hr))
					{
						
					}
				}

				CCmdDictionaryParam cmdParam;
			//	cmdParam.m_nMeterType = nMeterType;
				if (FAILED(hr = cmdParam.Open(Connection)))
					AtlMessageBox(NULL, FormatOLEDBMessageSimple().GetBuffer());
				else
				{
					while (S_OK == (hr = cmdParam.MoveNext()))
					{
						IItemPtr ptr (new CItemParam(i++));
						ptr->SetPropertyValue(DevicePropUid, any(cmdMeter.m_nID));
						ptr->SetPropertyValue(DevicePropTypeName, any(CString(cmdMeter.m_szName)));
						ptr->SetPropertyValue(DevicePropSubClass, any(cmdMeter.m_nSubClass));
						ptr->SetPropertyValue(DevicePropClass, any(cmdMeter.m_nClass));
						ptr->SetPropertyValue(DevicePropResource, any(cmdMeter.m_nResource));

						ptr->SetPropertyValue(DevicePropProtoId, any(cmdMeter.m_nProtocol));
						ptr->SetPropertyValue(DevicePropSelfPower, any(cmdMeter.m_lfSelf));
						GetModel().GetDevs().insert(make_pair(i, ptr));
					}

					if (FAILED(hr))
					{
						AtlMessageBox(NULL, FormatOLEDBMessageSimple().GetBuffer());					
					}
				}
			
		
		
		return S_OK;
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