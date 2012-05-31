#pragma once
#include "DeviceInfo.h"
#include "TagInfo.h"

class CDataModel
{
public:
	typedef std::map<int, IItemPtr> DeviceMap;
	typedef std::map<int, IItemPtr> ParamMap;

private:
	CDataModel(){}	
	static CDataModel m_instance;

public:
	static CDataModel& Instance(){
		return m_instance;
	}
	DeviceMap& GetDevs(){
		return m_mapDevs;
	}
	bool InsertParam(int nId, IItemPtr param){
		m_mapParams.insert(make_pair(nId, param));
		return true;
	}
protected:
	DeviceMap m_mapDevs;
	ParamMap m_mapParams;

};

CDataModel& GetModel();
