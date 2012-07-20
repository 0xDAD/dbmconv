#pragma once

#include "many.h"
#include <boost/shared_ptr.hpp>
#include <typeinfo>
#include <map>
#include <list>

using namespace std;
using namespace boost;

enum ItemType{
	ItemTypeNone,
	ItemTypeDevice,
	ItemTypeTag,
	ItemTypeOldImplNode,
	ItemTypeNewImplNode,
	ItemTypeTagClassNode,
	ItemTypeTagClass,
	ItemTypeDeviceObject,
	ItemTypeDevObjNode,
	ItemTypeDeviceTag,

};
enum BaseProperties{
	BasePropUndefined = -1,
	BasePropName = 0,
	UserPropsStart = 1
};

static const int ITEM_ID_INVALID = 0;
static const int ITEM_ID_ROOT = -1;

typedef std::map<int, many> CItemPropertyValueMap;

class ATL_NO_VTABLE IItem
{
public:
	virtual int GetID() const = 0;
	virtual CString GetName()  = 0;
	virtual bool SetName(LPCTSTR szName) = 0;

	virtual int GetParentID() const = 0;
	virtual int GetType() const = 0;

	virtual bool GetPropertyValues(CItemPropertyValueMap& rmapProperties) = 0;	
	virtual bool GetPropertyValueText(int nPropID, CString& strRepr) = 0;
	virtual bool HasPropertyValue(int nPropID) = 0;
	virtual bool GetPropertyValue(int nPropID, many& rValue) = 0;
	virtual bool SetPropertyValue(int nPropID, many& rValue) = 0;
};

template <class T, int t_nMinPropID, int t_nMaxPropID>
class CItemPropertiesMapBased
{
public:
	CItemPropertiesMapBased(){

	}
	static int GetMaxPropId(){
		return t_nMaxPropID;
	}
	static int GetMinPropId(){
		return t_nMinPropID;
	}
public:
	bool GetPropertyValues(CItemPropertyValueMap& rmapProperties) {
		T* pT = static_cast<T*>(this);
		many vValue;
		for (int i = t_nMinPropID; i <= t_nMaxPropID; ++i) {
			if (pT->GetPropertyValue(i, vValue))
				rmapProperties.insert(CItemPropertyValueMap::value_type(i, vValue));
		}
		return true;
	}
	bool HasPropertyValue(int nPropID) {
		if (nPropID < t_nMinPropID || nPropID > t_nMaxPropID)
			return false;
		return m_mapProperties.end() != m_mapProperties.find(nPropID);
	}
	bool GetPropertyValue(int nPropID, many& vValue) {
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
	bool GetPropertyValueText(int nPropID, CString& strRepr) {
		if (nPropID < t_nMinPropID || nPropID > t_nMaxPropID)
			return false;
		T* pT = static_cast<T*>(this);
		CItemPropertyValueMap::const_iterator it = m_mapProperties.find(nPropID);
		if (m_mapProperties.end() == it) {
			return false;
		}
		strRepr = it->second.to_string().c_str();
		return true;
	}
	bool SetPropertyValue(int nPropID, const many& rValue) {
		T* pT = static_cast<T*>(this);
		if(nPropID < t_nMinPropID || nPropID > t_nMaxPropID)
			return false;
		m_mapProperties[nPropID] = rValue;
		return true;
	}

private:
	CItemPropertyValueMap m_mapProperties;
};

template<class T, int t_nType, class TProperties>
class ATL_NO_VTABLE IItemImpl : public IItem
{
public: 
	typedef TProperties PropertyHolder;
public:
	IItemImpl(int nID, int nParentID) :
	  m_nID(nID), m_nParentID(nParentID)/*, m_strName(szName), m_bDisabled(false)*/ {
	  }
	virtual ~IItemImpl() {
//#ifdef DEBUG
//		CString str;
//		str.Format(L"del #%d\n", m_nID);
//		::OutputDebugString(str);
//#endif
	}
public:
	static int GetTypeID_static() {
		return t_nType;
	}

public:
	virtual CString GetName() {
		CString strRepr;
		m_PropertiesHolder.GetPropertyValueText(BasePropName, strRepr);
		return strRepr;
	}
	virtual bool SetName(LPCTSTR szName){
		return m_PropertiesHolder.SetPropertyValue(BasePropName, many(std::wstring(szName)));
	}
	virtual int GetType() const {
		return t_nType;
	}
	virtual int GetID() const {
		return m_nID;
	}
	virtual int GetParentID() const {
		return m_nParentID;
	}
	virtual bool GetPropertyValues(CItemPropertyValueMap& rmapProperties)
	{
		return m_PropertiesHolder.GetPropertyValues(rmapProperties);
	}
	virtual bool GetPropertyValue(int nPropID, many& rValue/*, ItemPropertyGetType nGetType*/){
		return m_PropertiesHolder.GetPropertyValue(nPropID, rValue);

	}
	virtual bool GetPropertyValueText(int nPropID, CString& strRepr){
		return m_PropertiesHolder.GetPropertyValueText(nPropID, strRepr);
	}
	virtual bool HasPropertyValue(int nPropID){
		return m_PropertiesHolder.HasPropertyValue(nPropID);
	}

	virtual bool SetPropertyValue(int nPropID, many& rValue){
		return m_PropertiesHolder.SetPropertyValue(nPropID, rValue);
	}

private:
	int m_nID;
	int m_nParentID;
	TProperties m_PropertiesHolder;
};

typedef boost::shared_ptr<IItem> IItemPtr;
typedef std::map<int, IItemPtr> ItemMap;
typedef std::list<IItemPtr> ItemList;
typedef std::map<int, ItemList> ItemTree;