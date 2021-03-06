#pragma once
#include "item.h"

enum ItemDeviceProperty {	
	DevicePropTypeName = BasePropName,
	DevicePropUid = UserPropsStart,
	DevicePropSubClass,
	DevicePropClass,
	DevicePropResource,

	DevicePropProtoId,	
	DevicePropSelfPower,

	DevicePropFirst = BasePropName,
	DevicePropLast = DevicePropSelfPower
};

class CDeviceProperties: 
	public CItemPropertiesMapBased<CDeviceProperties, DevicePropFirst, DevicePropLast>
{
	typedef CItemPropertiesMapBased<CDeviceProperties, DevicePropFirst, DevicePropLast> _Base;
public:
	using _Base::GetPropertyValue;
	using _Base::GetPropertyValues;
	using _Base::SetPropertyValue;
	using _Base::HasPropertyValue;
public:
	static inline LPCTSTR GetPropertyName(int nPropId){
		switch(nPropId){
			case DevicePropUid:
				return L"Uid";
			case DevicePropTypeName:
				return L"Name";
			case DevicePropSubClass:
				return L"SubClass";
			case DevicePropClass:
				return L"Class";
			case DevicePropResource: 
				return L"Resource";
			case DevicePropProtoId:
				return L"Proto";
			case DevicePropSelfPower:
				return L"SelfPower";
			default: 
				throw;
		}
	}
public:
	CDeviceProperties(){}
};

class CItemDevice: public IItemImpl<CItemDevice, ItemTypeDevice, CDeviceProperties>
{
	typedef IItemImpl<CItemDevice, ItemTypeDevice, CDeviceProperties> _Base;
public:
	CItemDevice (int nId, int nParent):_Base(nId, nParent){

	}
	
};

//class CDeviceInfo: IItemImpl<CDeviceInfo, 