#pragma once
#include "DeviceInfo.h"

class CDataModel
{
private:
	CDataModel(){}	
	static CDataModel m_instance;

public:
	static CDataModel& Instance(){
		return m_instance;
	}
	std::map<int, IItemPtr>& GetDevs(){
		return m_mapDevs;
	}
protected:
	std::map<int, IItemPtr> m_mapDevs;
};

CDataModel& GetModel();
