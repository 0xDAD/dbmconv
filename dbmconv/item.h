#pragma once

#include <boost/any.hpp>
#include <typeinfo>

using namespace std;
using namespace boost;


typedef std::map<int, boost::any> CItemPropertyValueMap;
class ATL_NO_VTABLE IItem
{
public:
	virtual int GetTypeID() = 0;
	//virtual type_info GetItemDataType() = 0; 
	virtual void GetCustomPropertyValues(CItemPropertyValueMap& rmapProperties);
	virtual bool GetPropertyValue(int nPropID, boost::any& rValue/*, ItemPropertyGetType nGetType*/) = 0;
	virtual bool HasPropertyValue(int nPropID) = 0;
	virtual bool GetPropertyValue(int nPropID, boost::any& rValue) = 0;
	virtual bool SetCustomPropertyValue(int nPropID, const boost::any& rValue) = 0;
	//virtual bool GetPropertyValueStr(int nPropID, CString& rstrValue) = 0;
	//virtual bool GetPropertyValueInt(int nPropID, int& rnValue) = 0;
};

template <class T, int t_nMinPropID, int t_nMaxPropID>
class CItemPropertiesMapBased
{
public:
	void GetCustomPropertyValues(CItemPropertyValueMap& rmapProperties) {
		T* pT = static_cast<T*>(this);
		boost::any vValue;
		for (int i = t_nMinPropID; i <= t_nMaxPropID; ++i) {
			if (pT->GetCustomPropertyValue(i, vValue))
				rmapProperties.insert(CItemPropertyValueMap::value_type(i, vValue));
		}
	}
	bool HasCustomPropertyValue(int nPropID) {
		if (nPropID < t_nMinPropID || nPropID > t_nMaxPropID)
			return false;
		return m_mapProperties.end() != m_mapProperties.find(nPropID);
	}
	bool GetCustomPropertyValue(int nPropID, boost::any& vValue) {
		if (nPropID < t_nMinPropID || nPropID > t_nMaxPropID)
			return false;
		T* pT = static_cast<T*>(this);
		//if (ItemPropGetTypeDefault == nGetType)
		//	return pT->GetDefaultCustomPropertyValue(nPropID, vValue);
		CItemPropertyValueMap::const_iterator it = m_mapProperties.find(nPropID);
		if (m_mapProperties.end() == it) {
			/*
			if (ItemPropGetTypeAlways == nGetType)
				return pT->GetDefaultCustomPropertyValue(nPropID, vValue);
			else*/
				return false;
		}
		vValue = it->second;
		return true;
	}
	bool SetCustomPropertyValue(int nPropID, const boost::any& rValue) {
		T* pT = static_cast<T*>(this);
		if(nPropID < t_nMinPropID || nPropID > t_nMaxPropID)
			return false;
		m_mapProperties[nPropID] = rValue;
		//CComVariant vValidValue;
		/*if (!pT->IsValidCustomPropertyValue(nPropID, vValue, vValidValue))
		return false;*/
		//vValidValue.Detach(&m_mapProperties[nPropID]);
		return true;
	}
	//bool RemoveCustomPropertyValue(int nPropID) {
	//	if (nPropID < t_nMinPropID || nPropID > t_nMaxPropID)
	//		return false;
	//	m_mapProperties.erase(nPropID);
	//	return true;
	//}
	//void SetCustomPropertyValues(const CItemPropertyValueMap& rmapValues) {
	//	T* pT = static_cast<T*>(this);
	//	for (CItemPropertyValueMap::const_iterator it = rmapValues.begin(); it != rmapValues.end(); ++it) {
	//		pT->SetCustomPropertyValue(it->first, it->second);
	//	}
	//}
private:
	CItemPropertyValueMap m_mapProperties;
};

template<class T, int t_nType, int t_nFlags, class TProperties>
class ATL_NO_VTABLE IItemImpl : public IItem
{
public:
	IItemImpl(int nID/*, int nParentID, LPCTSTR szName*/) :
	  m_nID(nID)/*, m_nParentID(nParentID), m_strName(szName), m_bDisabled(false)*/ {
	  }
//protected:
	~IItemImpl() {
	}
public:
	virtual int GetType() const {
		return t_nType;
	}
private:
	int m_nID;
	TProperties m_PropertiesHolder;
};