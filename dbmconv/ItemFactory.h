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
		case ItemDevice:
			rpItem = IItemPtr(new CItemDevice(nId, nParentId));
			break;
		case ItemTag:
			rpItem = IItemPtr(new CItemTag(nId, nParentId));
			break;
		case OldImplNode:
			rpItem = IItemPtr(new COldImplNode(nId, nParentId));
			break;
		case NewImplNode:
			rpItem = IItemPtr( new CNewImplNode(nId, nParentId));
			break;
		case TagClassNode:
			rpItem = IItemPtr( new CTagClassNode(nId, nParentId));
			break;
		case ItemTagClass:
			rpItem = IItemPtr( new CItemTagClass(nId, nParentId));
			break;
		default:
			return false;
		}
		return true;
	}
};