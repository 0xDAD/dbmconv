#pragma once
#include "item.h"

class CEmptyProperties:public CItemPropertiesMapBased<CEmptyProperties, BasePropName, BasePropName>
{
public:
	static inline LPCTSTR GetPropertyName(int nPropId){
		if(nPropId == BasePropName)
			return L"Name";
		else 
			return NULL;
	}
public:
	CEmptyProperties(){}
};


class CNewImplNode: public IItemImpl<CNewImplNode,  NewImplNode, CEmptyProperties>
{
	typedef IItemImpl<CNewImplNode,  NewImplNode, CEmptyProperties> _Base;
public:
	CNewImplNode (int nId, int nParent):_Base(nId, nParent){
	}
};

class COldImplNode: public IItemImpl<COldImplNode, OldImplNode , CEmptyProperties>
{
	typedef IItemImpl<COldImplNode, OldImplNode, CEmptyProperties> _Base;
public:
	COldImplNode (int nId, int nParent):_Base(nId, nParent){
	}
};

