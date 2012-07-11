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
TagAssignmentNone = 0,  // �� ������
TagAssignment1 = 	1 ,	// ��������+
TagAssignment2 = 	2 ,	// ��������+ ����� 1
TagAssignment3 = 	3 ,	// ��������+ ����� 2
TagAssignment4 = 	4 ,	// ��������+ ����� 3
TagAssignment5 =  	5 ,	// ��������+ ����� 4
TagAssignment6 =  	6 ,	// ��������+ ����� 5
TagAssignment7 =  	7 ,	// ��������+ ����� 6
TagAssignment8 =  	8 ,	// ��������+ ����� 7
TagAssignment9 =	9 ,	// ��������+ ����� 8
TagAssignment10 =	10,	// ����������+
TagAssignment11 =	11,	// ����������+ ����� 1
TagAssignment12 =	12,	// ����������+ ����� 2
TagAssignment13 =	13,	// ����������+ ����� 3
TagAssignment14 =	14,	// ����������+ ����� 4
TagAssignment15 =	15,	// ����������+ ����� 5
TagAssignment16 =	16,	// ����������+ ����� 6
TagAssignment17 =	17,	// ����������+ ����� 7
TagAssignment18 =	18,	// ����������+ ����� 8
TagAssignment19 =	19,	// �������� �������
TagAssignment20 =	20,	// �������� ������ �������������
TagAssignment21 =	21,	// ����������� ����� �������������
TagAssignment22 =	22,	// ����������� ������������� �� 1-�� ������
TagAssignment23 =	23,	// ����������� ������������� �� 2-�� ������
TagAssignment24 =	24,	// ����� ��������� ������� �����
TagAssignment25 =	25,	// ����������� ����� �������� ����
TagAssignment26 =	26,	// ����������� ����� ������� ����
TagAssignment27 =	27,	// ����������� ����� ����
TagAssignment28 =	28,	// ��������-
TagAssignment29 =	29,	// ��������- ����� 1
TagAssignment30 =	30,	// ��������- ����� 2
TagAssignment31 =	31,	// ��������- ����� 3
TagAssignment32 =	32,	// ��������- ����� 4
TagAssignment33 =	33,	// ��������- ����� 5
TagAssignment34 =	34,	// ��������- ����� 6
TagAssignment35 =	35,	// ��������- ����� 7
TagAssignment36 =	36,	// ��������- ����� 8
TagAssignment37 =	37,	// ����������-
TagAssignment38 =	38,	// ����������- ����� 1
TagAssignment39 =	39,	// ����������- ����� 2
TagAssignment40 =	40,	// ����������- ����� 3
TagAssignment41 =	41,	// ����������- ����� 4
TagAssignment42 =	42,	// ����������- ����� 5
TagAssignment43 =	43,	// ����������- ����� 6
TagAssignment44 =	44,	// ����������- ����� 7
TagAssignment45 =	45,	// ����������- ����� 8
TagAssignment46 =	46,	// �������� �������� ���� A
TagAssignment47 =	47,	// �������� �������� ���� B
TagAssignment48 =	48,	// �������� �������� ���� C
TagAssignment49 =	49,	// ���������� �������� ���� A
TagAssignment50 =	50,	// ���������� �������� ���� B
TagAssignment51 =	51,	// ���������� �������� ���� C
TagAssignment52 =	52,	// ��� ���� A
TagAssignment53 =	53,	// ��� ���� B
TagAssignment54 =	54,	// ��� ���� C
TagAssignment55 =	55,	// ���������� ���� A
TagAssignment56 =	56,	// ���������� ���� B
TagAssignment57 =	57,	// ���������� ���� C
TagAssignment61 =	61,	// ���� �������� ��������+ ����� 1
TagAssignment62 =	62,	// ���� �������� ��������+ ����� 2
TagAssignment63 =	63,	// ���� �������� ��������+ ����� 3
TagAssignment64 =	64,	// ���� �������� ��������+ ����� 4
TagAssignment65 =	65,	// ���� �������� ��������+ ����� 5
TagAssignment66 =	66,	// ���� �������� ��������+ ����� 6
TagAssignment67 =	67,	// ���� �������� ��������+ ����� 7
TagAssignment68 =	68,	// ���� �������� ��������+ ����� 8
TagAssignment69 =	69,	// ���� �������� ��������+ �����
TagAssignment70 =	70,	// ���� �������� ��������- �����
TagAssignment71 =	71,	// ���� �������� ��������- ����� 1
TagAssignment72 =	72,	// ���� �������� ��������- ����� 2
TagAssignment73 =	73,	// ���� �������� ��������- ����� 3
TagAssignment74 =	74,	// ���� �������� ��������- ����� 4
TagAssignment75 =	75,	// ���� �������� ��������- ����� 5
TagAssignment76 =	76,	// ���� �������� ��������- ����� 6
TagAssignment77 =	77,	// ���� �������� ��������- ����� 7
TagAssignment78 =	78,	// ���� �������� ��������- ����� 8
TagAssignment79 =	79,	// ���� ���������� ��������+ �����
TagAssignment80 =	80,	// ���� ���������� ��������+ ����� 1
TagAssignment81 =	81,	// ���� ���������� ��������+ ����� 2
TagAssignment82 =	82,	// ���� ���������� ��������+ ����� 3
TagAssignment83 =	83,	// ���� ���������� ��������+ ����� 4
TagAssignment84 =	84,	// ���� ���������� ��������+ ����� 5
TagAssignment85 =	85,	// ���� ���������� ��������+ ����� 6
TagAssignment86 =	86,	// ���� ���������� ��������+ ����� 7
TagAssignment87 =	87,	// ���� ���������� ��������+ ����� 8
TagAssignment88 =	88,	// ���� ���������� ��������- �����
TagAssignment89 =	89,	// ���� ���������� ��������- ����� 1
TagAssignment90 =	90,	// ���� ���������� ��������- ����� 2
TagAssignment91 =	91,	// ���� ���������� ��������- ����� 3
TagAssignment92 =	92,	// ���� ���������� ��������- ����� 4
TagAssignment93 =	93,	// ���� ���������� ��������- ����� 5
TagAssignment94 =	94,	// ���� ���������� ��������- ����� 6
TagAssignment95 =	95,	// ���� ���������� ��������- ����� 7
TagAssignment96 =	96,	// ���� ���������� ��������- ����� 8
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
			str = _T("���*�");
			break;
		case TagUnitsKW:
			str = _T("���");
			break;
		case TagUnitsV:
			str = _T("�");
			break;
		case TagUnitsA:
			str = _T("�");
			break;
		case TagUnitsHz:
			str = _T("��");
			break;
		case TagUnitsGr:
			str = _T("�C");
			break;
		case TagUnitsDT:
			str = _T("����/�����");
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
		
				case TagAssignment1 : 	str = L"��������+"; break;
				case TagAssignment2 : 	str = L"��������+ ����� 1"; break;
				case TagAssignment3 : 	str = L"��������+ ����� 2"; break;
				case TagAssignment4 : 	str = L"��������+ ����� 3"; break;
				case TagAssignment5 :  	str = L"��������+ ����� 4"; break;
				case TagAssignment6 :  	str = L"��������+ ����� 5"; break;
				case TagAssignment7 :  	str = L"��������+ ����� 6"; break;
				case TagAssignment8 :  	str = L"��������+ ����� 7"; break;
				case TagAssignment9 :	str = L"��������+ ����� 8"; break;
				case TagAssignment10 : 	str = L"����������+"; break;
				case TagAssignment11 : 	str = L"����������+ ����� 1"; break;
				case TagAssignment12 : 	str = L"����������+ ����� 2"; break;
				case TagAssignment13 : 	str = L"����������+ ����� 3"; break;
				case TagAssignment14 : 	str = L"����������+ ����� 4"; break;
				case TagAssignment15 : 	str = L"����������+ ����� 5"; break;
				case TagAssignment16 : 	str = L"����������+ ����� 6"; break;
				case TagAssignment17 : 	str = L"����������+ ����� 7"; break;
				case TagAssignment18 : 	str = L"����������+ ����� 8"; break;
				case TagAssignment19 : 	str = L"�������� �������"; break;
				case TagAssignment20 : 	str = L"�������� ������ �������������"; break;
				case TagAssignment21 : 	str = L"����������� ����� �������������"; break;
				case TagAssignment22 : 	str = L"����������� ������������� �� 1-�� ������ "; break;
				case TagAssignment23 : 	str = L"����������� ������������� �� 2-�� ������"; break;
				case TagAssignment24 : 	str = L"����� ��������� ������� �����"; break;
				case TagAssignment25 : 	str = L"����������� ����� �������� ����"; break;
				case TagAssignment26 : 	str = L"����������� ����� ������� ����"; break;
				case TagAssignment27 : 	str = L"����������� ����� ����"; break;
				case TagAssignment28 : 	str = L"��������-"; break;
				case TagAssignment29 : 	str = L"��������- ����� 1"; break;
				case TagAssignment30 : 	str = L"��������- ����� 2"; break;
				case TagAssignment31 : 	str = L"��������- ����� 3"; break;
				case TagAssignment32 : 	str = L"��������- ����� 4"; break;
				case TagAssignment33 : 	str = L"��������- ����� 5"; break;
				case TagAssignment34 : 	str = L"��������- ����� 6"; break;
				case TagAssignment35 : 	str = L"��������- ����� 7"; break;
				case TagAssignment36 : 	str = L"��������- ����� 8"; break;
				case TagAssignment37 : 	str = L"����������-"; break;
				case TagAssignment38 : 	str = L"����������- ����� 1"; break;
				case TagAssignment39 : 	str = L"����������- ����� 2"; break;
				case TagAssignment40 : 	str = L"����������- ����� 3"; break;
				case TagAssignment41 : 	str = L"����������- ����� 4"; break;
				case TagAssignment42 : 	str = L"����������- ����� 5"; break;
				case TagAssignment43 : 	str = L"����������- ����� 6"; break;
				case TagAssignment44 : 	str = L"����������- ����� 7"; break;
				case TagAssignment45 : 	str = L"����������- ����� 8"; break;
				case TagAssignment46 : 	str = L"�������� �������� ���� A"; break;
				case TagAssignment47 : 	str = L"�������� �������� ���� B"; break;
				case TagAssignment48 : 	str = L"�������� �������� ���� C"; break;
				case TagAssignment49 : 	str = L"���������� �������� ���� A"; break;
				case TagAssignment50 : 	str = L"���������� �������� ���� B"; break;
				case TagAssignment51 : 	str = L"���������� �������� ���� C"; break;
				case TagAssignment52 : 	str = L"��� ���� A"; break;
				case TagAssignment53 : 	str = L"��� ���� B"; break;
				case TagAssignment54 : 	str = L"��� ���� C"; break;
				case TagAssignment55 : 	str = L"���������� ���� A"; break;
				case TagAssignment56 : 	str = L"���������� ���� B"; break;
				case TagAssignment57 : 	str = L"���������� ���� C"; break;
				case TagAssignment61 : 	str = L"���� �������� ��������+ ����� 1"; break;
				case TagAssignment62 : 	str = L"���� �������� ��������+ ����� 2"; break;
				case TagAssignment63 : 	str = L"���� �������� ��������+ ����� 3"; break;
				case TagAssignment64 : 	str = L"���� �������� ��������+ ����� 4"; break;
				case TagAssignment65 : 	str = L"���� �������� ��������+ ����� 5"; break;
				case TagAssignment66 : 	str = L"���� �������� ��������+ ����� 6"; break;
				case TagAssignment67 : 	str = L"���� �������� ��������+ ����� 7"; break;
				case TagAssignment68 : 	str = L"���� �������� ��������+ ����� 8"; break;
				case TagAssignment69 : 	str = L"���� �������� ��������+ �����"; break;
				case TagAssignment70 : 	str = L"���� �������� ��������- �����"; break;
				case TagAssignment71 : 	str = L"���� �������� ��������- ����� 1"; break;
				case TagAssignment72 : 	str = L"���� �������� ��������- ����� 2"; break;
				case TagAssignment73 : 	str = L"���� �������� ��������- ����� 3"; break;
				case TagAssignment74 : 	str = L"���� �������� ��������- ����� 4"; break;
				case TagAssignment75 : 	str = L"���� �������� ��������- ����� 5"; break;
				case TagAssignment76 : 	str = L"���� �������� ��������- ����� 6"; break;
				case TagAssignment77 : 	str = L"���� �������� ��������- ����� 7"; break;
				case TagAssignment78 : 	str = L"���� �������� ��������- ����� 8"; break;
				case TagAssignment79 : 	str = L"���� ���������� ��������+ �����"; break;
				case TagAssignment80 : 	str = L"���� ���������� ��������+ ����� 1"; break;
				case TagAssignment81 : 	str = L"���� ���������� ��������+ ����� 2"; break;
				case TagAssignment82 : 	str = L"���� ���������� ��������+ ����� 3"; break;
				case TagAssignment83 : 	str = L"���� ���������� ��������+ ����� 4"; break;
				case TagAssignment84 : 	str = L"���� ���������� ��������+ ����� 5"; break;
				case TagAssignment85 : 	str = L"���� ���������� ��������+ ����� 6"; break;
				case TagAssignment86 : 	str = L"���� ���������� ��������+ ����� 7"; break;
				case TagAssignment87 : 	str = L"���� ���������� ��������+ ����� 8"; break;
				case TagAssignment88 : 	str = L"���� ���������� ��������- �����"; break;
				case TagAssignment89 : 	str = L"���� ���������� ��������- ����� 1"; break;
				case TagAssignment90 : 	str = L"���� ���������� ��������- ����� 2"; break;
				case TagAssignment91 : 	str = L"���� ���������� ��������- ����� 3"; break;
				case TagAssignment92 : 	str = L"���� ���������� ��������- ����� 4"; break;
				case TagAssignment93 :	str = L"���� ���������� ��������- ����� 5"; break;
				case TagAssignment94 :	str = L"���� ���������� ��������- ����� 6"; break;
				case TagAssignment95 :	str = L"���� ���������� ��������- ����� 7"; break;
				case TagAssignment96 :	str = L"���� ���������� ��������- ����� 8"; break;
		default:
			str.Format(_T("%d"), nValue);
			break;
		}
	}

private:
	static int sc_nNextUID;
};


