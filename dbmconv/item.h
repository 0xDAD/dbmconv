#pragma once

#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>
#include <typeinfo>
#include <map>

using namespace std;
using namespace boost;


typedef std::map<int, boost::any> CItemPropertyValueMap;

class ATL_NO_VTABLE IItem
{
public:
	virtual int GetTypeID()const = 0;
	virtual void GetPropertyValues(CItemPropertyValueMap& rmapProperties) = 0;
	virtual bool GetPropertyValue(int nPropID, boost::any& rValue/*, ItemPropertyGetType nGetType*/) = 0;
	virtual bool HasPropertyValue(int nPropID) = 0;
	virtual bool SetPropertyValue(int nPropID, boost::any& rValue) = 0;
};



template <class T, int t_nMinPropID, int t_nMaxPropID>
class CItemPropertiesMapBased
{
public:
	CItemPropertiesMapBased(){

	}
public:
	void GetPropertyValues(CItemPropertyValueMap& rmapProperties) {
		T* pT = static_cast<T*>(this);
		boost::any vValue;
		for (int i = t_nMinPropID; i <= t_nMaxPropID; ++i) {
			if (pT->GetPropertyValue(i, vValue))
				rmapProperties.insert(CItemPropertyValueMap::value_type(i, vValue));
		}
	}
	bool HasPropertyValue(int nPropID) {
		if (nPropID < t_nMinPropID || nPropID > t_nMaxPropID)
			return false;
		return m_mapProperties.end() != m_mapProperties.find(nPropID);
	}
	bool GetPropertyValue(int nPropID, boost::any& vValue) {
		if (nPropID < t_nMinPropID || nPropID > t_nMaxPropID)
			return false;
		T* pT = static_cast<T*>(this);
		CItemPropertyValueMap::const_iterator it = m_mapProperties.find(nPropID);
		if (m_mapProperties.end() == it) {
				return false;
		}
		vValue = it->second;
		return true;
	}
	bool SetPropertyValue(int nPropID, const boost::any& rValue) {
		T* pT = static_cast<T*>(this);
		if(nPropID < t_nMinPropID || nPropID > t_nMaxPropID)
			return false;
		m_mapProperties[nPropID] = rValue;
		return true;
	}
private:
	CItemPropertyValueMap m_mapProperties;
};

template<class T, int t_nType, /*int t_nFlags,*/ class TProperties>
class ATL_NO_VTABLE IItemImpl : public IItem
{
public:
	IItemImpl(int nID/*, int nParentID, LPCTSTR szName*/) :
	  m_nID(nID)/*, m_nParentID(nParentID), m_strName(szName), m_bDisabled(false)*/ {
	  }
	~IItemImpl() {
	}
public:
	virtual int GetTypeID() const {
		return t_nType;
	}
	virtual void GetPropertyValues(CItemPropertyValueMap& rmapProperties)
	{
		return m_PropertiesHolder.GetPropertyValues(rmapProperties);
	}
	virtual bool GetPropertyValue(int nPropID, boost::any& rValue/*, ItemPropertyGetType nGetType*/){
		return m_PropertiesHolder.GetPropertyValue(nPropID, rValue);

	}
	virtual bool HasPropertyValue(int nPropID){
		return m_PropertiesHolder.HasPropertyValue(nPropID);
	}

	virtual bool SetPropertyValue(int nPropID, boost::any& rValue){
		return m_PropertiesHolder.SetPropertyValue(nPropID, rValue);
	}
private:
	int m_nID;
	TProperties m_PropertiesHolder;
};

typedef boost::shared_ptr<IItem> IItemPtr;