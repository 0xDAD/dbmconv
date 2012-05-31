#pragma once
#include "item.h"

enum ItemDeviceProperty {
	DevicePropUid = 0,
	DevicePropTypeName,

	DevicePropSubClass,
	DevicePropClass,
	DevicePropResource,

	DevicePropProtoId,	
	DevicePropSelfPower,

	DevicePropFirst = DevicePropUid,
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
	CDeviceProperties(){}
};

class CItemDevice: public IItemImpl<CItemDevice, ItemDevice, CDeviceProperties>
{
	typedef IItemImpl<CItemDevice, ItemDevice, CDeviceProperties> _Base;
public:
	CItemDevice (int nId):_Base(nId){

	}

	
};

//class CDeviceInfo: IItemImpl<CDeviceInfo, 