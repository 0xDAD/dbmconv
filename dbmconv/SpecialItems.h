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


//class CNewImplNode: public IItemImpl<CNewImplNode,  NewImplNode, CEmptyProperties>
//{
//	typedef IItemImpl<CNewImplNode,  NewImplNode, CEmptyProperties> _Base;
//public:
//	CNewImplNode (int nId, int nParent):_Base(nId, nParent){
//	}
//};
//
//class COldImplNode: public IItemImpl<COldImplNode, OldImplNode , CEmptyProperties>
//{
//	typedef IItemImpl<COldImplNode, OldImplNode, CEmptyProperties> _Base;
//public:
//	COldImplNode (int nId, int nParent):_Base(nId, nParent){
//	}
//};
template <int t_nNodeType>
class CTreeNode: public IItemImpl<CTreeNode<t_nNodeType>, t_nNodeType , CEmptyProperties>{
	typedef IItemImpl<CTreeNode<t_nNodeType>, t_nNodeType , CEmptyProperties> _Base;
public:
	CTreeNode (int nId, int nParent):_Base(nId, nParent){
	}
};

typedef CTreeNode<ItemTypeNewImplNode> CNewImplNode;
typedef CTreeNode<ItemTypeOldImplNode> COldImplNode;
typedef CTreeNode<ItemTypeTagClassNode> CTagClassNode;
typedef CTreeNode<ItemTypeDevObjNode> CDevObjNode;