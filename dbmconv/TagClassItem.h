#pragma once

#include "item.h"

enum ItemTagClassProperty {		
	TagClassPropName = BasePropName,
	TagClassPropGuid = UserPropsStart,
	TagClassPropRefCnt,
	TagClassPropType,
	TagClassPropClass,
	TagClassPropUnitsType,
	TagClassPropUnitsName,
	TagClassPropCharacter,
	TagClassPropAssignment,
	TagClassPropAddressInMeter,

	TagClassPropAddress,

	TagClassPropFirst = BasePropName,
	TagClassPropLast = TagClassPropAddress
};

class CTagClassProperties: 
	public CItemPropertiesMapBased<CTagClassProperties, TagClassPropFirst, TagClassPropLast>
{
	typedef CItemPropertiesMapBased<CTagClassProperties, TagClassPropFirst, TagClassPropLast> _Base;
public:
	using _Base::GetPropertyValue;
	using _Base::GetPropertyValues;
	using _Base::SetPropertyValue;
	using _Base::HasPropertyValue;
public:
	static inline LPCTSTR GetPropertyName(int nPropId){
		switch(nPropId){
		case TagClassPropGuid: return L"Uid";
		case TagClassPropName: return L"Name";
		case TagClassPropType: return L"Type";
		case TagClassPropClass: return L"Class";
		case TagClassPropUnitsType: return L"UnitsType";
		case TagClassPropUnitsName: return L"UnitsName";
		case TagClassPropCharacter: return L"Character";
		case TagClassPropAssignment: return L"Assignment";
		case TagClassPropAddressInMeter: return L"AddressInMeter";
		case TagClassPropAddress: return L"Address";
		case TagClassPropRefCnt: return L"RefCnt";
		default: throw;
		}
	}
public:
	CTagClassProperties(){}
};

class CItemTagClass: public IItemImpl<CItemTagClass, ItemTagClass, CTagClassProperties>
{
	typedef IItemImpl<CItemTagClass, ItemTagClass, CTagClassProperties> _Base;
public:
	CItemTagClass (int nId, int nParent):_Base(nId, nParent){
	}
};

