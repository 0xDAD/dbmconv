//////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////////

class CDictionaryMeterAccessor
{
public:
	int m_nID;
	TCHAR m_szName[256];
	int m_nSubClass;
	int m_nClass;
	int m_nResource;
	int m_nProtocol;

	//BEGIN_PARAM_MAP(CDictionaryMeterAccessor)
	//	COLUMN_ENTRY(1, m_nID)
	//END_PARAM_MAP()

	BEGIN_COLUMN_MAP(CDictionaryMeterAccessor)
		COLUMN_ENTRY(1, m_nID)
		COLUMN_ENTRY(2, m_szName)
		// record #3 is skipped
		COLUMN_ENTRY(4, m_nSubClass)
		COLUMN_ENTRY(5, m_nClass)
		COLUMN_ENTRY(6, m_nResource)
		COLUMN_ENTRY(7, m_nProtocol)
	END_COLUMN_MAP()

	DEFINE_COMMAND_EX(CDictionaryMeterAccessor,
		L"select"
		L" Pribor.id,"
		L" Pribor.type,"
		L" Pribor.kod,"
		L" Pribor.id_subclass,"
		L" (select SubClass.id_class from SubClass where SubClass.id = Pribor.id_subclass) as id_class,"
		L" (select Class.id_res from Class where Class.id = (select SubClass.id_class from SubClass where SubClass.id = Pribor.id_subclass)) as id_res,"
		L" Pribor.id_protokol"
		L" from Pribor");
	// where Pribor.id=?");
};

class CDictionaryParamAccessor
{
public:
	int m_nID;
	DBSTATUS m_nIDStatus;
	TCHAR m_szName[256];
	int m_nType;				// 0- текущий, 1- архивный
	DBSTATUS m_nTypeStatus;
	int m_nClass;				// 0- основной, 1- дополнительный
	DBSTATUS m_nClassStatus;
	int m_nUnitsType;			// 0- аналоговый, 1- дискретный
	DBSTATUS m_nUnitsTypeStatus;
	TCHAR m_szUnitsName[256];
	DBSTATUS m_nCharacterStatus;
	int m_nCharacter;			// характер
	DBSTATUS m_nAssignmentStatus;
	int m_nAssignment;			// назначение
	TCHAR m_szAddressInCollector[256];

	int m_nMeterType;
	BEGIN_PARAM_MAP(CDictionaryParamAccessor)
		COLUMN_ENTRY(1, m_nMeterType)
	END_PARAM_MAP()

	BEGIN_COLUMN_MAP(CDictionaryParamAccessor)
		COLUMN_ENTRY_STATUS(1, m_nID, m_nIDStatus)
		COLUMN_ENTRY(2, m_szName)
		COLUMN_ENTRY_STATUS(3, m_nType, m_nTypeStatus)
		COLUMN_ENTRY_STATUS(4, m_nClass, m_nClassStatus)
		COLUMN_ENTRY_STATUS(5, m_nUnitsType, m_nUnitsTypeStatus)
		COLUMN_ENTRY(6, m_szUnitsName)
		COLUMN_ENTRY_STATUS(7, m_nCharacter, m_nCharacterStatus)
		COLUMN_ENTRY_STATUS(8, m_nAssignment, m_nAssignmentStatus)
		COLUMN_ENTRY(9, m_szAddressInCollector)
	END_COLUMN_MAP()

	DEFINE_COMMAND_EX(CDictionaryParamAccessor,
		L"select"
		L" id,"
		L" nazvanie,"
		L" type,"
		L" klass,"
		L" vid,"
		L" razmernost,"
		L" tag_name_id,"
		L" tag_type_id,"
		L" addr_in_uspd"
		L" from Haracter where id_pribor=?");
};

class CDictionaryAddressAccessor
{
public:
	int m_nAddress;
	DBSTATUS m_nAddressStatus;
	int m_nPosition;
	DBSTATUS m_nPositionStatus;

	int m_nParamID;
	BEGIN_PARAM_MAP(CDictionaryAddressAccessor)
		COLUMN_ENTRY(1, m_nParamID)
	END_PARAM_MAP()

	BEGIN_COLUMN_MAP(CDictionaryAddressAccessor)
		COLUMN_ENTRY_STATUS(1, m_nAddress, m_nAddressStatus)
		COLUMN_ENTRY_STATUS(2, m_nPosition, m_nPositionStatus)
	END_COLUMN_MAP()

	DEFINE_COMMAND_EX(CDictionaryAddressAccessor,
		L"select"
		L" addr,"
		L" id_local"
		L" from Address where id_har=?"
		L" order by id_local asc");
};

//////////////////////////////////////////////////////////////////////////

typedef CCommand<CAccessor<CDictionaryMeterAccessor> > CCmdDictionaryMeter;
typedef CCommand<CAccessor<CDictionaryParamAccessor> > CCmdDictionaryParam;
typedef CCommand<CAccessor<CDictionaryAddressAccessor> > CCmdDictionaryAddress;

//////////////////////////////////////////////////////////////////////////
