#pragma once
#include <atlstr.h>

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
