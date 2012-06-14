//////////////////////////////////////////////////////////////////////////
//
// Written by Yury Bura (splin@tut.by).
// Copyright© 2008 Yury Bura.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
//////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////////

#include <msxml2.h>

//////////////////////////////////////////////////////////////////////////

class CXMLDOMDocument
{
public:
	HRESULT Create() {
		m_spDoc.Release();
		return m_spDoc.CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER);
	}
	HRESULT Load(LPCWSTR pszFilePath) {
		m_spDoc.Release();
		HRESULT hr;
		if (FAILED(hr = m_spDoc.CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER)))
			return hr;
		VARIANT_BOOL bSuccessful;
		CComVariant vSource(pszFilePath);
		if (FAILED(hr = m_spDoc->load(vSource, &bSuccessful)))
			return hr;
		if (VARIANT_FALSE == bSuccessful)
			return E_FAIL;
		return S_OK;
	}
	template <class Q>
	HRESULT SelectFirst(LPCWSTR pwszName, Q** ppNode) {
		ATLASSERT(NULL != m_spDoc);
		if (NULL == m_spDoc)
			return E_FAIL;
		HRESULT hr;
		CComPtr<IXMLDOMNodeList> spList;
		if (FAILED(hr = m_spDoc->selectNodes(CComBSTR(pwszName), &spList)))
			return hr;
		long lCount = 0;
		if (FAILED(hr = spList->get_length(&lCount)))
			return hr;
		if (lCount == 0)
			return E_FAIL;
		CComPtr<IXMLDOMNode> spNode;
		if (FAILED(hr = spList->get_item(0, &spNode)))
			return hr;
		return spNode.QueryInterface(ppNode);
	}
	HRESULT Select(LPCWSTR pwszName, IXMLDOMNodeList** ppNodeList) {
		ATLASSERT(NULL != m_spDoc);
		if (NULL == m_spDoc)
			return E_FAIL;
		return m_spDoc->selectNodes(CComBSTR(pwszName), ppNodeList);
	}
	HRESULT Save(LPCWSTR pszFilePath) {
		ATLASSERT(NULL != m_spDoc);
		if (NULL == m_spDoc)
			return E_FAIL;
		CComVariant vDestination(pszFilePath);
		return m_spDoc->save(vDestination);
	}
	HRESULT CreateElement(LPCWSTR pwszName, IXMLDOMElement** ppElement) {
		ATLASSERT(NULL != m_spDoc);
		if (NULL == m_spDoc)
			return E_FAIL;
		HRESULT hr;
		CComPtr<IXMLDOMElement> spElement;
		if (FAILED(hr = m_spDoc->createElement(CComBSTR(pwszName), &spElement)))
			return hr;
		if (NULL != ppElement)
			*ppElement = spElement.Detach();
		return S_OK;
	}
	HRESULT CreateElementWithEndl(LPCWSTR pwszName, IXMLDOMElement** ppElement) {
		ATLASSERT(NULL != m_spDoc);
		if (NULL == m_spDoc)
			return E_FAIL;
		HRESULT hr;
		CComPtr<IXMLDOMElement> spElement;
		if (FAILED(hr = m_spDoc->createElement(CComBSTR(pwszName), &spElement)))
			return hr;
		if (FAILED(hr = AddTextNode(spElement, L"\n")))
			return hr;
		if (NULL != ppElement)
			*ppElement = spElement.Detach();
		return S_OK;
	}
	HRESULT CreateAttribute(LPCWSTR pwszName, const CComVariant& rvValue, IXMLDOMAttribute** ppAttribute = NULL) {
		ATLASSERT(NULL != m_spDoc);
		if (NULL == m_spDoc)
			return E_FAIL;
		HRESULT hr;
		CComPtr<IXMLDOMAttribute> spAttribute;
		if (FAILED(hr = m_spDoc->createAttribute(CComBSTR(pwszName), &spAttribute)))
			return hr;
		if (FAILED(hr = spAttribute->put_value(rvValue)))
			return hr;
		if (ppAttribute)
			*ppAttribute = spAttribute.Detach();
		return S_OK;
	}
	HRESULT SetAttributeValue(IXMLDOMElement* pElement, LPCWSTR pwszName, LPCTSTR pszValue) {
		return SetAttributeValue(pElement, pwszName, CComVariant(pszValue));
	}
	HRESULT SetAttributeValue(IXMLDOMElement* pElement, LPCWSTR pwszName, const CComVariant& rvValue) {
		ATLASSERT(NULL != pElement);
		if (NULL == pElement)
			return E_INVALIDARG;
		HRESULT hr;
		if (FAILED(hr = pElement->setAttribute(CComBSTR(pwszName), rvValue)))
			return hr;
		return S_OK;
	}
	HRESULT AppendProcessingInstruction(LPCWSTR pszTarget = L"xml", LPCWSTR pszData = L"version='1.0' encoding='utf-8'") {
		ATLASSERT(NULL != m_spDoc);
		if (NULL == m_spDoc)
			return E_FAIL;
		HRESULT hr;
		CComPtr<IXMLDOMProcessingInstruction> spProcessingInstruction;
		if (FAILED(hr = m_spDoc->createProcessingInstruction(CComBSTR(pszTarget), CComBSTR(pszData), &spProcessingInstruction)))
			return hr;
		if (FAILED(hr = m_spDoc->appendChild(spProcessingInstruction, NULL)))
			return hr;
		return S_OK;
	}
	HRESULT AppendNode(IXMLDOMNode* pNode) {
		ATLASSERT(NULL != m_spDoc);
		if (NULL == m_spDoc)
			return E_FAIL;
		HRESULT hr;
		if (FAILED(hr = m_spDoc->appendChild(pNode, NULL)))
			return hr;
		return S_OK;
	}
	HRESULT AppendNode(IXMLDOMNode* pParent, IXMLDOMNode* pNode) {
		ATLASSERT(NULL != m_spDoc);
		if (NULL == m_spDoc)
			return E_FAIL;
		HRESULT hr;
		if (FAILED(hr = pParent->appendChild(pNode, NULL)))
			return hr;
		if (FAILED(hr = AddTextNode(pParent, L"\n")))
			return hr;
		return S_OK;
	}
	HRESULT AddTextNode(IXMLDOMNode* pNode, LPCWSTR pszData, IXMLDOMText** ppText = NULL) {
		ATLASSERT(NULL != m_spDoc);
		if (NULL == m_spDoc)
			return E_FAIL;
		HRESULT hr;
		CComPtr<IXMLDOMText> spText;
		if (FAILED(hr = m_spDoc->createTextNode(CComBSTR(pszData), &spText)))
			return hr;
		if (FAILED(hr = pNode->appendChild(spText, NULL)))
			return hr;
		if (NULL != ppText)
			*ppText = spText.Detach();
		return S_OK;
	}
	HRESULT AddElementWithEndl(LPCWSTR pwszName, IXMLDOMElement** ppElement = NULL) {
		ATLASSERT(NULL != m_spDoc);
		if (NULL == m_spDoc)
			return E_FAIL;
		HRESULT hr;
		CComPtr<IXMLDOMElement> spElement;
		if (FAILED(hr = m_spDoc->createElement(CComBSTR(pwszName), &spElement)))
			return hr;
		if (FAILED(hr = AddTextNode(spElement, L"\n")))
			return hr;
		if (FAILED(hr = AppendNode(spElement)))
			return hr;
		if (NULL != ppElement)
			*ppElement = spElement.Detach();
		return S_OK;
	}
	HRESULT AddElement(IXMLDOMNode* pParent, LPCWSTR pwszName, IXMLDOMElement** ppElement = NULL) {
		ATLASSERT(NULL != m_spDoc);
		if (NULL == m_spDoc)
			return E_FAIL;
		HRESULT hr;
		CComPtr<IXMLDOMElement> spElement;
		if (FAILED(hr = CreateElement(pwszName, &spElement)))
			return hr;
		if (FAILED(hr = AppendNode(pParent, spElement)))
			return hr;
		if (NULL != ppElement)
			*ppElement = spElement.Detach();
		return S_OK;
	}
	HRESULT AddElementWithEndl(IXMLDOMNode* pParent, LPCWSTR pwszName, IXMLDOMElement** ppElement = NULL) {
		ATLASSERT(NULL != m_spDoc);
		if (NULL == m_spDoc)
			return E_FAIL;
		HRESULT hr;
		CComPtr<IXMLDOMElement> spElement;
		if (FAILED(hr = CreateElementWithEndl(pwszName, &spElement)))
			return hr;
		if (FAILED(hr = AppendNode(pParent, spElement)))
			return hr;
		if (NULL != ppElement)
			*ppElement = spElement.Detach();
		return S_OK;
	}
	HRESULT AddElementWithCdata(IXMLDOMNode* pParent, LPCWSTR pszElementName, LPCTSTR pszData, bool bAddEmptyData = true, IXMLDOMElement** ppElement = NULL) {
		ATLASSERT(NULL != m_spDoc);
		if (NULL == m_spDoc)
			return E_FAIL;
		HRESULT hr;
		CComPtr<IXMLDOMElement> spElement;
		if (FAILED(hr = m_spDoc->createElement(CComBSTR(pszElementName), &spElement)))
			return hr;
		CComBSTR bstrData(pszData);
		if (bAddEmptyData || 0 != bstrData.Length()) {
			CComPtr<IXMLDOMCDATASection> spCdata;
			if (FAILED(hr = m_spDoc->createCDATASection(bstrData, &spCdata)))
				return hr;
			if (FAILED(hr = spElement->appendChild(spCdata, NULL)))
				return hr;
		}
		if (FAILED(hr = AppendNode(pParent, spElement)))
			return hr;
		if (NULL != ppElement)
			*ppElement = spElement.Detach();
		return S_OK;
	}
	HRESULT AddElementWithText(IXMLDOMNode* pParent, LPCWSTR pszElementName, LPCTSTR pszData, bool bAddEmptyData = true, IXMLDOMElement** ppElement = NULL) {
		ATLASSERT(NULL != m_spDoc);
		if (NULL == m_spDoc)
			return E_FAIL;
		HRESULT hr;
		CComPtr<IXMLDOMElement> spElement;
		if (FAILED(hr = m_spDoc->createElement(CComBSTR(pszElementName), &spElement)))
			return hr;
		CComBSTR bstrData(pszData);
		if (bAddEmptyData || 0 != bstrData.Length()) {
			CComPtr<IXMLDOMText> spText;
			if (FAILED(hr = m_spDoc->createTextNode(bstrData, &spText)))
				return hr;
			if (FAILED(hr = spElement->appendChild(spText, NULL)))
				return hr;
		}
		if (FAILED(hr = AppendNode(pParent, spElement)))
			return hr;
		if (NULL != ppElement)
			*ppElement = spElement.Detach();
		return S_OK;
	}
private:
	CComPtr<IXMLDOMDocument> m_spDoc;
};
