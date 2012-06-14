#pragma once
#include "SAXContentHandler.h"



enum ParserState{
	StateStart,
	StateElementRoot,
	StateElement
};

class CDataModelContentHandler: public ISAXContentHandlerImpl<CDataModelContentHandler>
{
public:
	CDataModelContentHandler():m_curState(StateStart){

	}
	 ~CDataModelContentHandler(){}
public:
	 HRESULT STDMETHODCALLTYPE startDocument(){
		return S_OK;
	}

	 HRESULT STDMETHODCALLTYPE endDocument(){
		return S_OK;
	}
	 HRESULT STDMETHODCALLTYPE startElement( 
		/* [in] */ const wchar_t __RPC_FAR *pwchNamespaceUri,
		/* [in] */ int cchNamespaceUri,
		/* [in] */ const wchar_t __RPC_FAR *pwchLocalName,
		/* [in] */ int cchLocalName,
		/* [in] */ const wchar_t __RPC_FAR *pwchRawName,
		/* [in] */ int cchRawName,
		/* [in] */ ISAXAttributes __RPC_FAR *pAttributes){
			if(ReadElementName(pwchLocalName, cchLocalName)){

			}			
			return S_OK;
	}
	 HRESULT STDMETHODCALLTYPE characters( 
		 /* [in] */ const wchar_t __RPC_FAR *pwchChars,
		 /* [in] */ int cchChars)
	 {		 
		 return S_OK;
	 }
	 HRESULT STDMETHODCALLTYPE endElement( 
		/* [in] */ const wchar_t __RPC_FAR *pwchNamespaceUri,
		/* [in] */ int cchNamespaceUri,
		/* [in] */ const wchar_t __RPC_FAR *pwchLocalName,
		/* [in] */ int cchLocalName,
		/* [in] */ const wchar_t __RPC_FAR *pwchRawName,
		/* [in] */ int cchRawName){
			return S_OK;
	}
protected:
	bool ReadElementName(LPCWSTR pwchName, int nchCount){
		if(!pwchName)
			return false;
		LPWSTR pInternalBuf = NULL;
		if(m_strBuffer.GetAllocLength() < nchCount + 1)
			pInternalBuf = m_strBuffer.GetBufferSetLength(nchCount + 1);		
		else
			pInternalBuf = m_strBuffer.GetBuffer();

		wcsncpy_s(pInternalBuf, m_strBuffer.GetAllocLength(), pwchName, nchCount);
		pInternalBuf[nchCount + 1] = L'\0';
		return true;

	}
protected:
	ParserState m_curState;
	ItemMap m_mapDataItems;
	CString m_strBuffer;
};