#pragma once
#include "item.h"
#include "TagInfo.h"
#include "dlsres.h"

enum ItemDeviceObjProperty {		
	DeviceObjPropName = BasePropName,
	DeviceObjPropGuid = UserPropsStart,

	DeviceObjPropSubClass,
	DeviceObjPropClass,
	DeviceObjPropResource,
	DeviceObjPropProtoId,	
	DeviceObjPropSelfPower,

	DeviceObjRefDeviceInfo,

	DeviceObjPropFirst = BasePropName,
	DeviceObjPropLast = DeviceObjRefDeviceInfo
};


class CDeviceObjProperties: 
	public CItemPropertiesMapBased<CDeviceObjProperties, DeviceObjPropFirst, DeviceObjPropLast>
{
	typedef CItemPropertiesMapBased<CDeviceObjProperties, DeviceObjPropFirst, DeviceObjPropLast> _Base;
public:
	using _Base::GetPropertyValue;
	using _Base::GetPropertyValues;
	using _Base::SetPropertyValue;
	using _Base::HasPropertyValue;
public:
	static inline LPCTSTR GetPropertyName(int nPropId){
		switch(nPropId){
		case DeviceObjPropGuid: return L"Uid";
		case DeviceObjPropName: return L"Name";
		case DeviceObjPropSubClass:
			return L"SubClass";
		case DeviceObjPropClass:
			return L"Class";
		case DeviceObjPropResource: 
			return L"Resource";
		case DeviceObjPropProtoId:
			return L"Proto";
		case DeviceObjPropSelfPower:
			return L"SelfPower";
		case DeviceObjRefDeviceInfo: return L"RefDev";
		default: throw;
		}
	}
public:
	CDeviceObjProperties(){}
};

class CItemDeviceObj: public IItemImpl<CItemDeviceObj, ItemTypeDeviceObject, CDeviceObjProperties>
{
	typedef IItemImpl<CItemDeviceObj, ItemTypeDeviceObject, CDeviceObjProperties> _Base;
public:
	CItemDeviceObj (int nId, int nParent):_Base(nId, nParent){
		//SetPropertyValue(DeviceObjPropGuid, many(sc_nNextUID++));
	}

public:
	static void Type2String(int nValue, CString& str) {
		switch(nValue) {
		case TagTypeInvalid:
			str.LoadString(IDS_TAG_TYPE_INVALID);
			break;
		case TagTypeCurrent:
			str.LoadString(IDS_TAG_TYPE_CURRENT);
			break;
		case TagTypeArchival:
			str.LoadString(IDS_TAG_TYPE_ARCHIVAL);
			break;
		case TagTypeEvent:
			str.LoadString(IDS_TAG_TYPE_EVENT);
			break;
		default:
			str.Format(_T("%d"), nValue);
			break;
		}
	}

//private:
//	static int sc_nNextUID;
};


