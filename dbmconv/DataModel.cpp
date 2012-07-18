#include "stdafx.h"
#include "dlsres.h"
#include "DataModel.h"

CDataModel CDataModel::m_instance;
CDataModel& GetModel()
{
	return CDataModel::Instance();
}
