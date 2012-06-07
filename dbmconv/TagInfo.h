#pragma once
#include "item.h"

enum ItemTagProperty {
	
	TagPropGuid,
	TagPropName,
	TagPropCode,
	TagPropType,
	TagPropClass,
	TagPropUnitsType,
	TagPropUnitsName,
	TagPropCharacter,
	TagPropAssignment,
	TagPropAddressInMeter,

	TagPropAddress,

	TagPropFirst = TagPropGuid,
	TagPropLast = TagPropAddress
};

class CTagProperties: 
	public CItemPropertiesMapBased<CTagProperties, TagPropFirst, TagPropLast>
{
	typedef CItemPropertiesMapBased<CTagProperties, TagPropFirst, TagPropLast> _Base;
public:
	using _Base::GetPropertyValue;
	using _Base::GetPropertyValues;
	using _Base::SetPropertyValue;
	using _Base::HasPropertyValue;
public:
	static inline LPCTSTR GetPropertyName(int nPropId){
		switch(nPropId){
			case TagPropGuid: return L"Guid";
			case TagPropName: return L"Name";
			case TagPropCode: return L"Code";
			case TagPropType: return L"Type";
			case TagPropClass: return L"Class";
			case TagPropUnitsType: return L"UnitsType";
			case TagPropUnitsName: return L"UnitsName";
			case TagPropCharacter: return L"Character";
			case TagPropAssignment: return L"Assignment";
			case TagPropAddressInMeter: return L"AddressInMeter";
			case TagPropAddress: return L"Address";
		default: throw;
		}
	}
public:
	CTagProperties(){}
};

class CItemTag: public IItemImpl<CItemTag, ItemTag, CTagProperties>
{
	typedef IItemImpl<CItemTag, ItemTag, CTagProperties> _Base;
public:
	CItemTag (int nId):_Base(nId){
	}
};

