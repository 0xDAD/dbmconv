#pragma once
#include "DeviceInfo.h"
CDataModel& GetModel()
{
	return CDataModel::Instance();
}
class CDataModel
{
protected:
	CDataModel();
	static CDataModel m_instance;
public:
	static CDataModel& Instance(){
		return m_instance;
	}
public:
	

};
