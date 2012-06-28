#pragma  once
#include <wtypes.h>


class CResourceHolder
{
public:
	//////////////////////////////////////////////////////////////////////////
	// Properties constants

	inline static CString GetControlTextErrorStr() {
		return _LoadString(IDS_CONST_PROP_TEXT_ERROR);
	}
	inline static CString GetControlTextUnavailableStr() {
		return _LoadString(IDS_CONST_PROP_TEXT_UNAVAILABLE);
	}
	inline static CString GetControlTextNotSpecifiedStr() {
		return _LoadString(IDS_CONST_PROP_TEXT_NOTSPECIFIED);
	}
	inline static CString GetControlTextYesStr() {
		return _LoadString(IDS_CONST_PROP_TEXT_YES);
	}
	inline static CString GetControlTextNoStr() {
		return _LoadString(IDS_CONST_PROP_TEXT_NO);	}
private:

	inline static CString _LoadString(UINT nID) {
		CString str;
		ATLVERIFY(str.LoadString(nID));
		return str;
	}	
};