#pragma once
#include "item.h"
#include "TagInfo.h"
#include "dlsres.h"

enum ItemTagClassProperty {		
	TagClassPropName = BasePropName,
	TagClassPropGuid = UserPropsStart,
	TagClassPropRefCnt,
	TagClassPropType,
	TagClassPropClass,
	TagClassPropUnitsType,
	TagClassPropUnits,
	TagClassPropHar,
	TagClassPropAssignment,
	TagClassPropDirection,
	TagClassPropTariff,

	TagClassPropColAddress,

	TagClassPropFirst = BasePropName,
	TagClassPropLast = TagClassPropColAddress
};

enum ItemTagType {
	TagTypeInvalid = -1,
	TagTypeCurrent = 0,
	TagTypeArchival = 1,
	TagTypeEvent = 2,
};

//////////////////////////////////////////////////////////////////////////

enum ItemTagClass {
	TagClassInvalid = -1,
	TagClassBase = 0,
	TagClassOptional = 1,
	TagClassBase2 = 2
};

//////////////////////////////////////////////////////////////////////////

enum ItemTagUnitsType {
	TagUnitsTypeInvalid = -1,
	TagUnitsTypeAnalog = 0,
	TagUnitsTypeDiscrete = 1,
	TagUnitsTypeSpecial = 2,

	TagUnitsTypeFirst = TagUnitsTypeInvalid,
	TagUnitsTypeLast = TagUnitsTypeSpecial
};

//////////////////////////////////////////////////////////////////////////

enum ItemTagUnits {
	TagUnitsNone = -1,
	TagUnitsKWh = 0,
	TagUnitsKW,
	TagUnitsV,
	TagUnitsA,
	TagUnitsHz,
	TagUnitsGr,
	TagUnitsDT,

	TagUnitsFirst = TagUnitsNone,
	TagUnitsLast = TagUnitsDT
};
//////////////////////////////////////////////////////////////////////////

enum ItemTagCharacter {
	TagCharacterInvalid = -1,
	TagCharacterNone = 0,
	TagCharacter03Minute = 1,
	TagCharacter30Minute = 2,
	TagCharacterHour = 3,
	TagCharacterDay = 4,
	TagCharacterMonth = 5,
	TagCharacterBeginOfMonth = 6,
	TagCharacterSummary = 7,
	TagCharacterBeginOfDay = 8,

	TagCharacterFirst = TagCharacterInvalid,
	TagCharacterLast = TagCharacterBeginOfDay
};


enum ItemTagTariff {
	TagTariffInvalid = -1,
	TagTariffNone = 0,
	TagTariffOverall = 1,
	TagTariffT1 = 2,
	TagTariffT2 = 3,
	TagTariffT3 = 4,
	TagTariffT4 = 5,
	TagTariffT5 = 6,
	TagTariffT6 = 7,
	TagTariffT7 = 8,
	TagTariffT8 = 9,
};
enum ItemTagAssignment {

TagAssignmentInvalid = -1 ,	
TagAssignmentNone = 0,  // не задано
TagAssignment1 = 	1 ,	// Активная+
TagAssignment2 = 	2 ,	// Активная+ тариф 1
TagAssignment3 = 	3 ,	// Активная+ тариф 2
TagAssignment4 = 	4 ,	// Активная+ тариф 3
TagAssignment5 =  	5 ,	// Активная+ тариф 4
TagAssignment6 =  	6 ,	// Активная+ тариф 5
TagAssignment7 =  	7 ,	// Активная+ тариф 6
TagAssignment8 =  	8 ,	// Активная+ тариф 7
TagAssignment9 =	9 ,	// Активная+ тариф 8
TagAssignment10 =	10,	// Реактивная+
TagAssignment11 =	11,	// Реактивная+ тариф 1
TagAssignment12 =	12,	// Реактивная+ тариф 2
TagAssignment13 =	13,	// Реактивная+ тариф 3
TagAssignment14 =	14,	// Реактивная+ тариф 4
TagAssignment15 =	15,	// Реактивная+ тариф 5
TagAssignment16 =	16,	// Реактивная+ тариф 6
TagAssignment17 =	17,	// Реактивная+ тариф 7
TagAssignment18 =	18,	// Реактивная+ тариф 8
TagAssignment19 =	19,	// Тепловая энергия
TagAssignment20 =	20,	// Объемный расход теплоносителя
TagAssignment21 =	21,	// Накопленный объем теплоносителя
TagAssignment22 =	22,	// Температура теплоносителя по 1-му каналу
TagAssignment23 =	23,	// Температура теплоносителя по 2-му каналу
TagAssignment24 =	24,	// Время наработки прибора учета
TagAssignment25 =	25,	// Накопленный объем холодной воды
TagAssignment26 =	26,	// Накопленный объем горячей воды
TagAssignment27 =	27,	// Накопленный объем газа
TagAssignment28 =	28,	// Активная-
TagAssignment29 =	29,	// Активная- тариф 1
TagAssignment30 =	30,	// Активная- тариф 2
TagAssignment31 =	31,	// Активная- тариф 3
TagAssignment32 =	32,	// Активная- тариф 4
TagAssignment33 =	33,	// Активная- тариф 5
TagAssignment34 =	34,	// Активная- тариф 6
TagAssignment35 =	35,	// Активная- тариф 7
TagAssignment36 =	36,	// Активная- тариф 8
TagAssignment37 =	37,	// Реактивная-
TagAssignment38 =	38,	// Реактивная- тариф 1
TagAssignment39 =	39,	// Реактивная- тариф 2
TagAssignment40 =	40,	// Реактивная- тариф 3
TagAssignment41 =	41,	// Реактивная- тариф 4
TagAssignment42 =	42,	// Реактивная- тариф 5
TagAssignment43 =	43,	// Реактивная- тариф 6
TagAssignment44 =	44,	// Реактивная- тариф 7
TagAssignment45 =	45,	// Реактивная- тариф 8
TagAssignment46 =	46,	// Активная мощность фаза A
TagAssignment47 =	47,	// Активная мощность фаза B
TagAssignment48 =	48,	// Активная мощность фаза C
TagAssignment49 =	49,	// Реактивная мощность фаза A
TagAssignment50 =	50,	// Реактивная мощность фаза B
TagAssignment51 =	51,	// Реактивная мощность фаза C
TagAssignment52 =	52,	// Ток фаза A
TagAssignment53 =	53,	// Ток фаза B
TagAssignment54 =	54,	// Ток фаза C
TagAssignment55 =	55,	// Напряжение фаза A
TagAssignment56 =	56,	// Напряжение фаза B
TagAssignment57 =	57,	// Напряжение фаза C
TagAssignment61 =	61,	// Макс активная мощность+ тариф 1
TagAssignment62 =	62,	// Макс активная мощность+ тариф 2
TagAssignment63 =	63,	// Макс активная мощность+ тариф 3
TagAssignment64 =	64,	// Макс активная мощность+ тариф 4
TagAssignment65 =	65,	// Макс активная мощность+ тариф 5
TagAssignment66 =	66,	// Макс активная мощность+ тариф 6
TagAssignment67 =	67,	// Макс активная мощность+ тариф 7
TagAssignment68 =	68,	// Макс активная мощность+ тариф 8
TagAssignment69 =	69,	// Макс активная мощность+ общая
TagAssignment70 =	70,	// Макс активная мощность- общая
TagAssignment71 =	71,	// Макс активная мощность- тариф 1
TagAssignment72 =	72,	// Макс активная мощность- тариф 2
TagAssignment73 =	73,	// Макс активная мощность- тариф 3
TagAssignment74 =	74,	// Макс активная мощность- тариф 4
TagAssignment75 =	75,	// Макс активная мощность- тариф 5
TagAssignment76 =	76,	// Макс активная мощность- тариф 6
TagAssignment77 =	77,	// Макс активная мощность- тариф 7
TagAssignment78 =	78,	// Макс активная мощность- тариф 8
TagAssignment79 =	79,	// Макс реактивная мощность+ общая
TagAssignment80 =	80,	// Макс реактивная мощность+ тариф 1
TagAssignment81 =	81,	// Макс реактивная мощность+ тариф 2
TagAssignment82 =	82,	// Макс реактивная мощность+ тариф 3
TagAssignment83 =	83,	// Макс реактивная мощность+ тариф 4
TagAssignment84 =	84,	// Макс реактивная мощность+ тариф 5
TagAssignment85 =	85,	// Макс реактивная мощность+ тариф 6
TagAssignment86 =	86,	// Макс реактивная мощность+ тариф 7
TagAssignment87 =	87,	// Макс реактивная мощность+ тариф 8
TagAssignment88 =	88,	// Макс реактивная мощность- общая
TagAssignment89 =	89,	// Макс реактивная мощность- тариф 1
TagAssignment90 =	90,	// Макс реактивная мощность- тариф 2
TagAssignment91 =	91,	// Макс реактивная мощность- тариф 3
TagAssignment92 =	92,	// Макс реактивная мощность- тариф 4
TagAssignment93 =	93,	// Макс реактивная мощность- тариф 5
TagAssignment94 =	94,	// Макс реактивная мощность- тариф 6
TagAssignment95 =	95,	// Макс реактивная мощность- тариф 7
TagAssignment96 =	96,	// Макс реактивная мощность- тариф 8
};
class CTagClassProperties: 
	public CItemPropertiesMapBased<CTagClassProperties, TagClassPropFirst, TagClassPropLast>
{
	typedef CItemPropertiesMapBased<CTagClassProperties, TagClassPropFirst, TagClassPropLast> _Base;
public:
	using _Base::GetPropertyValue;
	using _Base::GetPropertyValues;
	using _Base::SetPropertyValue;
	using _Base::HasPropertyValue;
public:
	static inline LPCTSTR GetPropertyName(int nPropId){
		switch(nPropId){
		case TagClassPropGuid: return L"Uid";
		case TagClassPropName: return L"Name";
		case TagClassPropType: return L"Type";
		case TagClassPropClass: return L"Class";
		case TagClassPropUnitsType: return L"UType";
		case TagClassPropUnits: return L"Units";
		case TagClassPropHar: return L"Haracter";
		case TagClassPropTariff: return L"Tariff";
		case TagClassPropDirection: return L"Dir";
		case TagClassPropAssignment: return L"Assign";
		case TagClassPropColAddress: return L"Address";
		case TagClassPropRefCnt: return L"RefCnt";
		default: throw;
		}
	}
public:
	CTagClassProperties(){}
};
	  
class CItemTagClass: public IItemImpl<CItemTagClass, ItemTypeTagClass, CTagClassProperties>
{
	typedef IItemImpl<CItemTagClass, ItemTypeTagClass, CTagClassProperties> _Base;
public:
	CItemTagClass (int nId, int nParent):_Base(nId, nParent){
		SetPropertyValue(TagClassPropGuid, many(sc_nNextUID++));
	}

public:
	static void Type2String(int nValue, CString& str) {
		switch(nValue) {
		case TagTypeInvalid:
			str.LoadString(IDS_TAG_TYPE_INVALID);
			break;
		case TagTypeCurrent:
			str.LoadString(IDS_TAG_TYPE_CURRENT);
			break;
		case TagTypeArchival:
			str.LoadString(IDS_TAG_TYPE_ARCHIVAL);
			break;
		case TagTypeEvent:
			str.LoadString(IDS_TAG_TYPE_EVENT);
			break;
		default:
			str.Format(_T("%d"), nValue);
			break;
		}
	}
	static void Class2String(int nValue, CString& str) {
		switch(nValue) {
		case TagClassInvalid:
			str.LoadString(IDS_TAG_CLASS_INVALID);
			break;
		case TagClassBase:
			str.LoadString(IDS_TAG_CLASS_BASE);
			break;
		case TagClassOptional:
			str.LoadString(IDS_TAG_CLASS_OPTIONAL);
			break;
		case TagClassBase2:
			str.LoadString(IDS_TAG_CLASS_BASE2);
			break;
		default:
			str.Format(_T("%d"), nValue);
			break;
		}
	}
	static void UnitsType2String(int nValue, CString& str) {
		switch(nValue) {
		case TagUnitsTypeInvalid:
			str.LoadString(IDS_TAG_UNITS_TYPE_INVALID);
			break;
		case TagUnitsTypeAnalog:
			str.LoadString(IDS_TAG_UNITS_TYPE_ANALOG);
			break;
		case TagUnitsTypeDiscrete:
			str.LoadString(IDS_TAG_UNITS_TYPE_DISCRETE);
			break;
		default:
			str.Format(_T("%d"), nValue);
			break;
		}
	}
	static void Units2String(int nValue, CString& str) {
		switch(nValue) {
		case TagUnitsNone:
			str.LoadString(IDS_TAG_UNITS_TYPE_INVALID);
			break;
		case TagUnitsKWh:
			str = _T("кВт*ч");
			break;
		case TagUnitsKW:
			str = _T("кВт");
			break;
		case TagUnitsV:
			str = _T("В");
			break;
		case TagUnitsA:
			str = _T("А");
			break;
		case TagUnitsHz:
			str = _T("Гц");
			break;
		case TagUnitsGr:
			str = _T("°C");
			break;
		case TagUnitsDT:
			str = _T("дата/время");
			break;				
		default:
			str.Format(_T("%d"), nValue);
			break;
		}
	}
	static void Character2String(int nValue, CString& str) {
		switch(nValue) {
		case TagCharacterInvalid:
		case TagCharacterNone:
			str.LoadString(IDS_TAG_CHAR_INVALID);
			break;
		case TagCharacter30Minute:
			str.LoadString(IDS_TAG_CHAR_30MIN);
			break;
		case TagCharacterHour:
			str.LoadString(IDS_TAG_CHAR_HOUR);
			break;
		case TagCharacterDay:
			str.LoadString(IDS_TAG_CHAR_DAY);
			break;
		case TagCharacterMonth:
			str.LoadString(IDS_TAG_CHAR_MONTH);
			break;
		case TagCharacterBeginOfMonth:
			str.LoadString(IDS_TAG_CHAR_BEG_OF_MONTH);
			break;
		case TagCharacterSummary:
			str.LoadString(IDS_TAG_CHAR_SUM);
			break;
		case TagCharacterBeginOfDay:
			str.LoadString(IDS_TAG_CHAR_BEG_OF_DAY);
			break;
		default:
			str.Format(_T("%d"), nValue);
			break;
		}
	}
	static void Assignment2String(int nValue, CString& str) {
		switch(nValue) {
		case TagAssignmentInvalid:
		case TagAssignmentNone:
			str.LoadString(IDS_TAG_ASSiGNMENT_INVALID);
			break;
		
				case TagAssignment1 : 	str = L"Активная+"; break;
				case TagAssignment2 : 	str = L"Активная+ тариф 1"; break;
				case TagAssignment3 : 	str = L"Активная+ тариф 2"; break;
				case TagAssignment4 : 	str = L"Активная+ тариф 3"; break;
				case TagAssignment5 :  	str = L"Активная+ тариф 4"; break;
				case TagAssignment6 :  	str = L"Активная+ тариф 5"; break;
				case TagAssignment7 :  	str = L"Активная+ тариф 6"; break;
				case TagAssignment8 :  	str = L"Активная+ тариф 7"; break;
				case TagAssignment9 :	str = L"Активная+ тариф 8"; break;
				case TagAssignment10 : 	str = L"Реактивная+"; break;
				case TagAssignment11 : 	str = L"Реактивная+ тариф 1"; break;
				case TagAssignment12 : 	str = L"Реактивная+ тариф 2"; break;
				case TagAssignment13 : 	str = L"Реактивная+ тариф 3"; break;
				case TagAssignment14 : 	str = L"Реактивная+ тариф 4"; break;
				case TagAssignment15 : 	str = L"Реактивная+ тариф 5"; break;
				case TagAssignment16 : 	str = L"Реактивная+ тариф 6"; break;
				case TagAssignment17 : 	str = L"Реактивная+ тариф 7"; break;
				case TagAssignment18 : 	str = L"Реактивная+ тариф 8"; break;
				case TagAssignment19 : 	str = L"Тепловая энергия"; break;
				case TagAssignment20 : 	str = L"Объемный расход теплоносителя"; break;
				case TagAssignment21 : 	str = L"Накопленный объем теплоносителя"; break;
				case TagAssignment22 : 	str = L"Температура теплоносителя по 1-му каналу "; break;
				case TagAssignment23 : 	str = L"Температура теплоносителя по 2-му каналу"; break;
				case TagAssignment24 : 	str = L"Время наработки прибора учета"; break;
				case TagAssignment25 : 	str = L"Накопленный объем холодной воды"; break;
				case TagAssignment26 : 	str = L"Накопленный объем горячей воды"; break;
				case TagAssignment27 : 	str = L"Накопленный объем газа"; break;
				case TagAssignment28 : 	str = L"Активная-"; break;
				case TagAssignment29 : 	str = L"Активная- тариф 1"; break;
				case TagAssignment30 : 	str = L"Активная- тариф 2"; break;
				case TagAssignment31 : 	str = L"Активная- тариф 3"; break;
				case TagAssignment32 : 	str = L"Активная- тариф 4"; break;
				case TagAssignment33 : 	str = L"Активная- тариф 5"; break;
				case TagAssignment34 : 	str = L"Активная- тариф 6"; break;
				case TagAssignment35 : 	str = L"Активная- тариф 7"; break;
				case TagAssignment36 : 	str = L"Активная- тариф 8"; break;
				case TagAssignment37 : 	str = L"Реактивная-"; break;
				case TagAssignment38 : 	str = L"Реактивная- тариф 1"; break;
				case TagAssignment39 : 	str = L"Реактивная- тариф 2"; break;
				case TagAssignment40 : 	str = L"Реактивная- тариф 3"; break;
				case TagAssignment41 : 	str = L"Реактивная- тариф 4"; break;
				case TagAssignment42 : 	str = L"Реактивная- тариф 5"; break;
				case TagAssignment43 : 	str = L"Реактивная- тариф 6"; break;
				case TagAssignment44 : 	str = L"Реактивная- тариф 7"; break;
				case TagAssignment45 : 	str = L"Реактивная- тариф 8"; break;
				case TagAssignment46 : 	str = L"Активная мощность фаза A"; break;
				case TagAssignment47 : 	str = L"Активная мощность фаза B"; break;
				case TagAssignment48 : 	str = L"Активная мощность фаза C"; break;
				case TagAssignment49 : 	str = L"Реактивная мощность фаза A"; break;
				case TagAssignment50 : 	str = L"Реактивная мощность фаза B"; break;
				case TagAssignment51 : 	str = L"Реактивная мощность фаза C"; break;
				case TagAssignment52 : 	str = L"Ток фаза A"; break;
				case TagAssignment53 : 	str = L"Ток фаза B"; break;
				case TagAssignment54 : 	str = L"Ток фаза C"; break;
				case TagAssignment55 : 	str = L"Напряжение фаза A"; break;
				case TagAssignment56 : 	str = L"Напряжение фаза B"; break;
				case TagAssignment57 : 	str = L"Напряжение фаза C"; break;
				case TagAssignment61 : 	str = L"Макс активная мощность+ тариф 1"; break;
				case TagAssignment62 : 	str = L"Макс активная мощность+ тариф 2"; break;
				case TagAssignment63 : 	str = L"Макс активная мощность+ тариф 3"; break;
				case TagAssignment64 : 	str = L"Макс активная мощность+ тариф 4"; break;
				case TagAssignment65 : 	str = L"Макс активная мощность+ тариф 5"; break;
				case TagAssignment66 : 	str = L"Макс активная мощность+ тариф 6"; break;
				case TagAssignment67 : 	str = L"Макс активная мощность+ тариф 7"; break;
				case TagAssignment68 : 	str = L"Макс активная мощность+ тариф 8"; break;
				case TagAssignment69 : 	str = L"Макс активная мощность+ общая"; break;
				case TagAssignment70 : 	str = L"Макс активная мощность- общая"; break;
				case TagAssignment71 : 	str = L"Макс активная мощность- тариф 1"; break;
				case TagAssignment72 : 	str = L"Макс активная мощность- тариф 2"; break;
				case TagAssignment73 : 	str = L"Макс активная мощность- тариф 3"; break;
				case TagAssignment74 : 	str = L"Макс активная мощность- тариф 4"; break;
				case TagAssignment75 : 	str = L"Макс активная мощность- тариф 5"; break;
				case TagAssignment76 : 	str = L"Макс активная мощность- тариф 6"; break;
				case TagAssignment77 : 	str = L"Макс активная мощность- тариф 7"; break;
				case TagAssignment78 : 	str = L"Макс активная мощность- тариф 8"; break;
				case TagAssignment79 : 	str = L"Макс реактивная мощность+ общая"; break;
				case TagAssignment80 : 	str = L"Макс реактивная мощность+ тариф 1"; break;
				case TagAssignment81 : 	str = L"Макс реактивная мощность+ тариф 2"; break;
				case TagAssignment82 : 	str = L"Макс реактивная мощность+ тариф 3"; break;
				case TagAssignment83 : 	str = L"Макс реактивная мощность+ тариф 4"; break;
				case TagAssignment84 : 	str = L"Макс реактивная мощность+ тариф 5"; break;
				case TagAssignment85 : 	str = L"Макс реактивная мощность+ тариф 6"; break;
				case TagAssignment86 : 	str = L"Макс реактивная мощность+ тариф 7"; break;
				case TagAssignment87 : 	str = L"Макс реактивная мощность+ тариф 8"; break;
				case TagAssignment88 : 	str = L"Макс реактивная мощность- общая"; break;
				case TagAssignment89 : 	str = L"Макс реактивная мощность- тариф 1"; break;
				case TagAssignment90 : 	str = L"Макс реактивная мощность- тариф 2"; break;
				case TagAssignment91 : 	str = L"Макс реактивная мощность- тариф 3"; break;
				case TagAssignment92 : 	str = L"Макс реактивная мощность- тариф 4"; break;
				case TagAssignment93 :	str = L"Макс реактивная мощность- тариф 5"; break;
				case TagAssignment94 :	str = L"Макс реактивная мощность- тариф 6"; break;
				case TagAssignment95 :	str = L"Макс реактивная мощность- тариф 7"; break;
				case TagAssignment96 :	str = L"Макс реактивная мощность- тариф 8"; break;
		default:
			str.Format(_T("%d"), nValue);
			break;
		}
	}

private:
	static int sc_nNextUID;
};


