#pragma once
#include "SAXContentHandler.h"
#include "XMLDefs.h"
#include "ItemFactory.h"


static const int sc_nMaxAttrValueLength = 50;

enum ParserState{
	StateStart,
	StateRoot,
	StateItem,
	StateItemProp,
	StateError
};

class CDataModelContentHandler: public ISAXContentHandlerImpl<CDataModelContentHandler>
{
public:
	CDataModelContentHandler():
	  m_curState(StateStart),
	  m_nCurId(ITEM_ID_INVALID),
	  m_nCurPropId(BasePropUndefined)
	  {

	  }
	 ~CDataModelContentHandler(){}
public:
	 HRESULT STDMETHODCALLTYPE startDocument(){
		 m_curState = StateStart;
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
			if(!_ParseElem(pwchLocalName, cchLocalName)){
				m_curState = _Error();
				return E_FAIL;
			}
			if (m_strElem == sc_cwszXmlNodeRoot)
					m_curState = _StartRoot();
			else
				if (m_strElem == sc_cwszXmlNodeItem)
					m_curState = _StartItem(pAttributes);
				else
					if(m_strElem == sc_cwszXmlNodeItemProperty)
						m_curState = _StartProperty(pAttributes);
					else				
						m_curState = _Error();		

			if(m_curState == StateError)
				return E_FAIL;

			return S_OK;
	}
	 HRESULT STDMETHODCALLTYPE characters( 
		 /* [in] */ const wchar_t __RPC_FAR *pwchChars,
		 /* [in] */ int cchChars)
	 {		 
		 if(m_curState == StateItemProp)
			 if(!_ParseData(pwchChars, cchChars))
			 return E_FAIL;
		 return S_OK;
	 }
	 HRESULT STDMETHODCALLTYPE endElement( 
		/* [in] */ const wchar_t __RPC_FAR *pwchNamespaceUri,
		/* [in] */ int cchNamespaceUri,
		/* [in] */ const wchar_t __RPC_FAR *pwchLocalName,
		/* [in] */ int cchLocalName,
		/* [in] */ const wchar_t __RPC_FAR *pwchRawName,
		/* [in] */ int cchRawName){
			if(!_ParseElem(pwchLocalName, cchLocalName)){
				m_curState = _Error();
				return E_FAIL;
			}
			if (m_strElem == sc_cwszXmlNodeRoot)
				m_curState = _EndRoot();
			else
				if (m_strElem == sc_cwszXmlNodeItem)
					m_curState = _EndItem();
				else
					if(m_strElem == sc_cwszXmlNodeItemProperty)
						m_curState = _EndProperty();
					else				
						m_curState = _Error();	
			if(m_curState == StateError)
				return E_FAIL;
			return S_OK;
	}
protected:
	bool _ParseBuf(LPCWSTR pBufSrc, int nchCountSrc, CString& strDataDst){
		if(!pBufSrc)
			return false;
		LPWSTR pInternalBuf = NULL;
		if(strDataDst.GetAllocLength() < nchCountSrc + 1)
			pInternalBuf = strDataDst.GetBufferSetLength(nchCountSrc + 1);		
		else
			pInternalBuf = strDataDst.GetBuffer();

		wcsncpy_s(pInternalBuf, strDataDst.GetAllocLength(), pBufSrc, nchCountSrc);
		pInternalBuf[nchCountSrc + 1] = L'\0';
		return true;
	}

	bool _ParseElem(LPCWSTR pwchName, int nchCount){
		return _ParseBuf(pwchName, nchCount, m_strElem);
	}

	bool _ParseData(LPCWSTR pwchCharacters, int nchCount){
		return _ParseBuf(pwchCharacters, nchCount, m_strData);
	}
	
	bool _ParseAttr(ISAXAttributes* pAttr, LPCWSTR pwchAttrName, CString& strValue){
		int nchVal = 0;
		LPCWSTR pszVal = NULL;
		if(!pAttr || FAILED(pAttr->getValueFromName(NULL, 0, pwchAttrName, wcslen(pwchAttrName), &pszVal, &nchVal)))
			return false;

		return _ParseBuf(pszVal, nchVal, strValue);		
	}

protected:
	ParserState _Error(){
		return StateError;
	}

	ParserState _StartRoot()
	{
		if(m_curState != StateStart)
			return _Error();
		else
			return StateRoot;
	}

	ParserState _StartItem(ISAXAttributes* pAttr)
	{
		if(m_curState != StateRoot || !pAttr)
			return _Error();

		int nAttrCount = 0;
		if(FAILED(pAttr->getLength(&nAttrCount)) || nAttrCount != 3)
			return _Error();

		CString strAttrVal;
		if(!_ParseAttr(pAttr, sc_cwszXmlNodeItemAttrType, strAttrVal))
			return _Error();

		int nItemType = StrToInt(strAttrVal);

		if(!_ParseAttr(pAttr, sc_cwszXmlNodeItemAttrID, strAttrVal))
			return _Error();
		
		int nItemId = StrToInt(strAttrVal);

		if(!_ParseAttr(pAttr, sc_cwszXmlNodeItemAttrParID, strAttrVal))
			return _Error();

		int nItemParId = StrToInt(strAttrVal);

		IItemPtr pItem;
		if(!CItemFactory::CreateItem(nItemType, nItemId, nItemParId, pItem))
			return _Error();

		m_mapDataItems.insert(ItemMap::value_type(nItemId, pItem));
		m_nCurId = nItemId;
		return StateItem;
	}

	ParserState _StartProperty(ISAXAttributes* pAttr){
		if(m_curState != StateItem || !pAttr || m_nCurId == ITEM_ID_INVALID)
			return _Error();

		int nAttrCount = 0;
		if(FAILED(pAttr->getLength(&nAttrCount)) || nAttrCount != 1)
			return _Error();

		CString strAttrVal;
		if(!_ParseAttr(pAttr, sc_cwszXmlNodeItemPropertyAttrID, strAttrVal))
			return _Error();

		m_nCurPropId = StrToInt(strAttrVal);	

		return StateItemProp;
	}

	ParserState _EndRoot(){
		if(m_curState != StateRoot)
			return _Error();
		return StateStart;
	}

	ParserState _EndItem(){
		if(m_curState != StateItem || m_nCurId == ITEM_ID_INVALID)
			return _Error();
		m_nCurId = ITEM_ID_INVALID;
		m_strElem.Empty();
		return StateRoot;
	}

	ParserState _EndProperty(){
		if(m_curState != StateItemProp || m_nCurId == ITEM_ID_INVALID)
			return _Error();
		if(m_mapDataItems.find(m_nCurId) == m_mapDataItems.end() || !m_mapDataItems[m_nCurId])
			return _Error();

		IItemPtr pItem = m_mapDataItems[m_nCurId];
		std::wstring  val = m_strData.GetBuffer();
		if(!pItem->SetPropertyValue(m_nCurPropId, many(val)))
			return _Error();
		m_strData.Empty();
		return StateItem;
	}
public:
	ItemMap& GetItems(){
		return m_mapDataItems;
	}
protected:
	int m_nCurId;
	int m_nCurPropId;

	ParserState m_curState;
	ItemMap m_mapDataItems;

	CString m_strElem;
	CString m_strData;	

};