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
	CTagProperties(){}
};

class CItemTag: public IItemImpl<CItemTag, ItemTag, CTagProperties>
{
	typedef IItemImpl<CItemTag, ItemTag, CTagProperties> _Base;
public:
	CItemTag (int nId):_Base(nId){
	}
};

