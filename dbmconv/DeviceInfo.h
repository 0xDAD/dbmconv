#pragma once
#include "item.h"
class CDeviceInfo
{
public:
	CDeviceInfo(){
		m_nId = 0;
		m_nProtoId = 0;
		m_lfSelfPower = 0.;		
	}
	CDeviceInfo(int nId, LPCTSTR szName, int nProto):m_strName(szName), m_nId(nId), m_nProtoId(nProto){

	}

protected:
	int m_nId;
	int m_nProtoId;
	double m_lfSelfPower;

	int m_nClassOld;
	int m_nSubClassOld;
	int m_nResourceOld;


	CString m_strName;
};

//template <class T, class 
//class IAbstractItem