#pragma once
#include "item.h"
#include "TagInfo.h"
#include "dlsres.h"

enum ItemDeviceTagProperty {		
	DeviceTagPropName = BasePropName,
	DeviceTagPropGuid = UserPropsStart,
	DeviceTagClassId,
	DeviceTagRefOld,

	DeviceTagPropFirst = BasePropName,
	DeviceTagPropLast = DeviceTagRefOld
};


class CDeviceTagProperties: 
	public CItemPropertiesMapBased<CDeviceTagProperties, DeviceTagPropFirst, DeviceTagPropLast>
{
	typedef CItemPropertiesMapBased<CDeviceTagProperties, DeviceTagPropFirst, DeviceTagPropLast> _Base;
public:
	using _Base::GetPropertyValue;
	using _Base::GetPropertyValues;
	using _Base::SetPropertyValue;
	using _Base::HasPropertyValue;
public:
	static inline LPCTSTR GetPropertyName(int nPropId){
		switch(nPropId){
		case DeviceTagPropGuid: return L"Uid";
		case DeviceTagPropName: return L"Name";
		case DeviceTagClassId: return L"TagClass";
			//case DeviceTagPropType: return L"Type";
			//case DeviceTagPropClass: return L"Class";
			//case DeviceTagPropUnitsType: return L"UType";
			//case DeviceTagPropUnits: return L"Units";
			//case DeviceTagPropHar: return L"Haracter";
			//case DeviceTagPropTariff: return L"Tariff";
			//case DeviceTagPropDirection: return L"Dir";
			//case DeviceTagPropAssignment: return L"Assign";
		case DeviceTagRefOld: return L"RefTag";
		default: throw;
		}
	}
public:
	CDeviceTagProperties(){}
};

class CItemDeviceTag: public IItemImpl<CItemDeviceTag, ItemTypeDeviceTag, CDeviceTagProperties>
{
	typedef IItemImpl<CItemDeviceTag, ItemTypeDeviceTag, CDeviceTagProperties> _Base;
public:
	CItemDeviceTag (int nId, int nParent):_Base(nId, nParent){
		//SetPropertyValue(DeviceTagPropGuid, many(sc_nNextUID++));
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


