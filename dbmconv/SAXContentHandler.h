#pragma once
#include <MsXml2.h>

template <class T>
class ATL_NO_VTABLE ISAXContentHandlerImpl : public ISAXContentHandler  
{
public:
	ISAXContentHandlerImpl(){}
	virtual ~ISAXContentHandlerImpl(){}

	// This must be correctly implemented, if your handler must be a COM Object (in this example it does not)
	long __stdcall QueryInterface(const struct _GUID &,void ** ){return 0;}
	unsigned long __stdcall AddRef(void){return 0;}
	unsigned long __stdcall Release(void){return 0;}

	 HRESULT STDMETHODCALLTYPE putDocumentLocator( 
		/* [in] */ ISAXLocator __RPC_FAR *pLocator){
			return S_OK;
	}
		

	 HRESULT STDMETHODCALLTYPE startDocument( void){
		T* pT = static_cast<T*>(this);
		return pT->startDocument();
	}

	 HRESULT STDMETHODCALLTYPE endDocument( void){
		T* pT = static_cast<T*>(this);
		return pT->endDocument();
	}

	 HRESULT STDMETHODCALLTYPE startPrefixMapping( 
		/* [in] */ const wchar_t __RPC_FAR *pwchPrefix,
		/* [in] */ int cchPrefix,
		/* [in] */ const wchar_t __RPC_FAR *pwchUri,
		/* [in] */ int cchUri){
			return S_OK;
	}

	 HRESULT STDMETHODCALLTYPE endPrefixMapping( 
		/* [in] */ const wchar_t __RPC_FAR *pwchPrefix,
		/* [in] */ int cchPrefix){
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
			T* pT = static_cast<T*>(this);
			return pT->startElement(pwchNamespaceUri, cchNamespaceUri, pwchLocalName, cchLocalName, pwchRawName, cchRawName,pAttributes); 
	}

	 HRESULT STDMETHODCALLTYPE endElement( 
		/* [in] */ const wchar_t __RPC_FAR *pwchNamespaceUri,
		/* [in] */ int cchNamespaceUri,
		/* [in] */ const wchar_t __RPC_FAR *pwchLocalName,
		/* [in] */ int cchLocalName,
		/* [in] */ const wchar_t __RPC_FAR *pwchRawName,
		/* [in] */ int cchRawName){
			T* pT = static_cast<T*>(this);
			return pT->endElement(pwchNamespaceUri, cchNamespaceUri, pwchLocalName, cchLocalName, pwchRawName, cchRawName); 

	}

	 HRESULT STDMETHODCALLTYPE characters( 
		/* [in] */ const wchar_t __RPC_FAR *pwchChars,
		/* [in] */ int cchChars){
			return S_OK;
	}

	 HRESULT STDMETHODCALLTYPE ignorableWhitespace( 
		/* [in] */ const wchar_t __RPC_FAR *pwchChars,
		/* [in] */ int cchChars){
			return S_OK;
	}

	 HRESULT STDMETHODCALLTYPE processingInstruction( 
		/* [in] */ const wchar_t __RPC_FAR *pwchTarget,
		/* [in] */ int cchTarget,
		/* [in] */ const wchar_t __RPC_FAR *pwchData,
		/* [in] */ int cchData){
			return S_OK;
	}

	 HRESULT STDMETHODCALLTYPE skippedEntity( 
		/* [in] */ const wchar_t __RPC_FAR *pwchName,
		/* [in] */ int cchName){
			return S_OK;
	}

};

