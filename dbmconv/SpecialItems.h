#pragma once
#include "item.h"

class CEmptyProperties
{
public:
	static int GetMinPropId(){
		return 0;
	}
	static int GetMaxPropId(){
		return 0;
	}
public:
	bool GetPropertyValues(CItemPropertyValueMap& rmapProperties) {	
		return false;
	}
	bool HasPropertyValue(int nPropID) {
		return false;
	}
	bool GetPropertyValue(int nPropID, many& vValue) {		
		return false;
	}
	bool GetPropertyValueText(int nPropID, CString& strRepr) {		
		return false;
	}
	bool SetPropertyValue(int nPropID, const many& rValue) {		
		return false;
	}

};

class COldImplNode: IItemImpl<COldImplNode,  OldImplNode, CEmptyProperties>
{
	typedef IItemImpl<COldImplNode,  OldImplNode, CEmptyProperties> _Base;
public:
	COldImplNode (int nId, int nParent):_Base(nId, nParent){
	}
};


class CNewImplNode: IItemImpl<CNewImplNode,  NewImplNode, CEmptyProperties>
{
	typedef IItemImpl<CNewImplNode,  NewImplNode, CEmptyProperties> _Base;
public:
	CNewImplNode (int nId, int nParent):_Base(nId, nParent){
	}
};