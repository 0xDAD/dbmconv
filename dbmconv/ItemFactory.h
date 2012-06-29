#pragma once
#include "item.h"
#include "SpecialItems.h"
#include "DeviceInfo.h"
#include "TagInfo.h"
#include "TagClassItem.h"

class CItemFactory{
protected:
	CItemFactory(){}
	virtual ~CItemFactory(){};
public:
	static bool CreateItem(int nType, int nId, int nParentId, IItemPtr& rpItem){
		switch(nType){
		case ItemTypeDevice:
			rpItem = IItemPtr(new CItemDevice(nId, nParentId));
			break;
		case ItemTypeTag:
			rpItem = IItemPtr(new CItemTag(nId, nParentId));
			break;
		case ItemTypeOldImplNode:
			rpItem = IItemPtr(new COldImplNode(nId, nParentId));
			break;
		case ItemTypeNewImplNode:
			rpItem = IItemPtr( new CNewImplNode(nId, nParentId));
			break;
		case ItemTypeTagClassNode:
			rpItem = IItemPtr( new CTagClassNode(nId, nParentId));
			break;
		case ItemTypeTagClass:
			rpItem = IItemPtr( new CItemTagClass(nId, nParentId));
			break;
		default:
			return false;
		}
		return true;
	}
};