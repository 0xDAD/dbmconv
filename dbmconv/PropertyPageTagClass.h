//////////////////////////////////////////////////////////////////////////

#pragma once
#include <atldlgs.h>
#include "DataModel.h"
#include "ResourceHolder.h"
#include <boost/function.hpp>
//////////////////////////////////////////////////////////////////////////

#ifndef _WTL_NEW_PAGE_NOTIFY_HANDLERS
#error CPropertyPageTagClass requires _WTL_NEW_PAGE_NOTIFY_HANDLERS
#endif // _WTL_NEW_PAGE_NOTIFY_HANDLERS

#define DIFF_ARGS_IDX -2
//////////////////////////////////////////////////////////////////////////

class CPropertyPageTagClass : public CPropertyPageImpl<CPropertyPageTagClass>,
	public CDialogResize<CPropertyPageTagClass>,
	public CWinDataExchange<CPropertyPageTagClass>
{
	typedef CPropertyPageImpl<CPropertyPageTagClass> baseClass;
	typedef CDialogResize<CPropertyPageTagClass> baseResizeClass;
public:
	CPropertyPageTagClass(const std::vector<int>& rvctItemIDs)
		: m_rvctItemIDs(rvctItemIDs)
		, m_nChanged(0)
		 {
	}

	bool IsSingleItemSelected() const {
		return 1 == m_rvctItemIDs.size();
	}

public:
	enum { IDD = IDD_PP_TAGCLASS};

	enum cChange {
		cPropName = 0x0001,
		cPropGuid  = 0x0002,
		cPropRefCnt = 0x0004,
		cPropType = 0x0008,
		cPropClass = 0x0010,
		cPropUnitsTypeId = 0x0020,	
		cPropHar = 0x0040,
		cDirection = 0x0080,
		cTariff = 0x0100,
		cPropAddress = 0x0200,
		cPropUnits = 0x400,
		cPropFamily = 0x800,
	};

protected:
	BEGIN_MSG_MAP(CPropertyPageTagClass)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_EDIT_NAME, EN_CHANGE, OnChangeEditName)
		//COMMAND_HANDLER(IDC_EDT_DESCRIPTION, EN_CHANGE, OnChangeEditDescription)
		//COMMAND_HANDLER(IDC_CHK_DISABLED, BN_CLICKED, OnChangeCheckDisabled)
		CHAIN_MSG_MAP(baseResizeClass)
		CHAIN_MSG_MAP(baseClass)		
		COMMAND_HANDLER(IDC_EDIT_UID, EN_CHANGE, OnEnChangeEditUid)
		COMMAND_HANDLER(IDC_COMBO_TYPE, CBN_SELCHANGE, OnCbnSelchangeComboType)
		COMMAND_HANDLER(IDC_COMBO_CLASS, CBN_SELCHANGE, OnCbnSelchangeComboClass)
		COMMAND_HANDLER(IDC_COMBO_FAMILY, CBN_SELCHANGE, OnCbnSelchangeComboFamily)
		COMMAND_HANDLER(IDC_COMBO_UNITTYPE, CBN_SELCHANGE, OnCbnSelchangeComboUnittype)
		COMMAND_HANDLER(IDC_COMBO_UNITS, CBN_SELCHANGE, OnCbnSelchangeComboUnits)
		COMMAND_HANDLER(IDC_COMBO_CHARACTER, CBN_SELCHANGE, OnCbnSelchangeComboCharacter)
		COMMAND_HANDLER(IDC_EDIT_COLADDR, EN_CHANGE, OnEnChangeEditColaddr)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DLGRESIZE_MAP(CPropertyPageTagClass)
		DLGRESIZE_CONTROL(IDC_EDIT_NAME, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_COMBO_TYPE, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_EDIT_UID, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_COMBO_CLASS, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_COMBO_FAMILY, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_COMBO_DIRECTION, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_COMBO_TARIFF, DLSZ_SIZE_X)

		DLGRESIZE_CONTROL(IDC_COMBO_TARIFF, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_COMBO_UNITS, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_COMBO_UNITTYPE, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_COMBO_CHARACTER, DLSZ_SIZE_X)
		//DLGRESIZE_CONTROL(IDC_EDT_DESCRIPTION, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		//DLGRESIZE_CONTROL(IDC_CHK_DISABLED, DLSZ_MOVE_Y)
	END_DLGRESIZE_MAP()

	BEGIN_DDX_MAP(CPropertyPageTagClass)
		DDX_TEXT(IDC_EDIT_NAME, m_strName)
		DDX_TEXT(IDC_EDIT_COLADDR, m_strColAddr)
		DDX_INT(IDC_EDIT_UID, m_nID)

		DDX_CONTROL_HANDLE(IDC_COMBO_TYPE, m_wndCmbType)
		DDX_CONTROL_HANDLE(IDC_COMBO_CLASS, m_wndCmbClass)
		DDX_CONTROL_HANDLE(IDC_COMBO_FAMILY, m_wndCmbAssignment)
		DDX_CONTROL_HANDLE(IDC_COMBO_DIRECTION, m_wndCmbDirection)
		DDX_CONTROL_HANDLE(IDC_COMBO_TARIFF, m_wndCmbTariff)
		DDX_CONTROL_HANDLE(IDC_COMBO_UNITS, m_wndCmbUnits)
		DDX_CONTROL_HANDLE(IDC_COMBO_UNITTYPE, m_wndCmbUnitType)
		DDX_CONTROL_HANDLE(IDC_COMBO_CHARACTER, m_wndCmbCharact)
		//DDX_TEXT(IDC_EDT_FULL_NAME, m_strFullName)
		//DDX_TEXT(IDC_EDT_DESCRIPTION, m_strDescription)
		//DDX_CHECK(IDC_CHK_DISABLED, m_nDisabled)
	END_DDX_MAP()

	// Messages
public:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// Initialize resize
		DlgResize_Init(false);
		// Initialize data exchange
		DoDataExchange(DDX_LOAD);

		_InitCombo(m_wndCmbType, TagTypeInvalid, TagTypeEvent, &CItemTagClass::Type2String);
		_InitCombo(m_wndCmbAssignment, TagAssignmentNone, TagAssignment96, &CItemTagClass::Assignment2String);
		_InitCombo(m_wndCmbClass, TagClassInvalid, TagClassBase2, &CItemTagClass::Class2String);
		_InitCombo(m_wndCmbUnits, TagUnitsFirst, TagUnitsLast, &CItemTagClass::Units2String);
		_InitCombo(m_wndCmbUnitType, TagUnitsTypeFirst, TagUnitsTypeLast, &CItemTagClass::UnitsType2String);
		_InitCombo(m_wndCmbCharact, TagCharacterFirst, TagCharacterLast, &CItemTagClass::Character2String);

		InitData();
		m_nChanged = 0;
		SetModified(FALSE);
		return TRUE;
	}
	LRESULT OnChangeEditName(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_nChanged |= cPropName;
		SetModified();
		return 0;
	}
	LRESULT CPropertyPageTagClass::OnEnChangeEditUid(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)	
	{
		m_nChanged |= cPropGuid;
		SetModified();
		return 0;
	}


	LRESULT CPropertyPageTagClass::OnCbnSelchangeComboType(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_nChanged |= cPropType;
		SetModified();		
		return 0;
	}


	LRESULT CPropertyPageTagClass::OnCbnSelchangeComboClass(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_nChanged |= cPropClass;
		SetModified();
		return 0;
	}


	LRESULT CPropertyPageTagClass::OnCbnSelchangeComboFamily(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_nChanged |= cPropFamily;
		SetModified();
		return 0;
	}


	LRESULT CPropertyPageTagClass::OnCbnSelchangeComboUnittype(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_nChanged |= cPropUnitsTypeId;
		SetModified();
		return 0;
	}


	LRESULT CPropertyPageTagClass::OnCbnSelchangeComboUnits(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_nChanged |= cPropUnits;
		SetModified();
		return 0;
	}


	LRESULT CPropertyPageTagClass::OnCbnSelchangeComboCharacter(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_nChanged |= cPropHar;
		SetModified();
		return 0;
	}


	LRESULT CPropertyPageTagClass::OnEnChangeEditColaddr(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_nChanged |= cPropAddress;
		SetModified();
		return 0;
	}

	//LRESULT OnChangeEditDescription(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//{
	//	m_nChanged |= cDescription;
	//	SetModified();
	//	return 0;
	//}
	//LRESULT OnChangeCheckDisabled(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//{
	//	// Note: Auto flag for check box isn't specified
	//	if (!DoDataExchange(DDX_SAVE, IDC_CHK_DISABLED))
	//		return 0;
	//	m_nDisabled = !m_nDisabled;
	//	if (!DoDataExchange(DDX_LOAD, IDC_CHK_DISABLED))
	//		return 0;
	//	m_nChanged |= cDisabled;
	//	SetModified();
	//	return 0;
	//}

public:
	int OnApply()
	{
		if (!DoDataExchange(DDX_SAVE))
			return PSNRET_INVALID_NOCHANGEPAGE;
		/*	if (m_nChanged & cName) {
		if (IsSingleItemSelected()) {
		if (!GetNamespace().SetItemPropertyValue(m_rvctItemIDs.front(), OPCTL::ItemPropName, m_strName)) {
		_XData data = { ddxDataNull };
		OnDataValidateError(IDC_EDT_NAME, TRUE, data);
		return PSNRET_INVALID_NOCHANGEPAGE;
		}
		m_strFullName = GetNamespace().GetItemFullNameSimple(m_rvctItemIDs.front());
		ATLVERIFY(DoDataExchange(DDX_LOAD, IDC_EDT_FULL_NAME));
		}
		m_nChanged &= ~cName;
		}
		if (m_nChanged & cDescription) {
		if (!GetNamespace().SetMultiItemPropertyValue(m_rvctItemIDs, OPCTL::ItemPropDescription, m_strDescription)) {
		_XData data = { ddxDataNull };
		OnDataValidateError(IDC_EDT_DESCRIPTION, TRUE, data);
		return PSNRET_INVALID_NOCHANGEPAGE;
		}
		m_nChanged &= ~cDescription;
		}
		if (m_nChanged & cDisabled) {
		if (m_nDisabled < 2) {
		if (!GetNamespace().SetMultiItemPropertyValue(m_rvctItemIDs, OPCTL::ItemPropDisabled, CComVariant(0 != m_nDisabled))) {
		_XData data = { ddxDataNull };
		OnDataValidateError(IDC_CHK_DISABLED, TRUE, data);
		return PSNRET_INVALID_NOCHANGEPAGE;
		}
		}
		m_nChanged &= ~cDisabled;
		}
		ATLASSERT(0 == m_nChanged);*/
		SetModified(FALSE);
		// PSNRET_NOERROR = apply OK
		// PSNRET_INVALID = apply not OK, return to this page
		// PSNRET_INVALID_NOCHANGEPAGE = apply not OK, don't change focus
		return PSNRET_NOERROR;
	}
protected:
	void InitData() 
	{
		// Is single item selected?
		if (IsSingleItemSelected()) {
			IItemPtr ptr;
			if(GetModel().GetItem(m_rvctItemIDs.front(), ptr) && ptr->GetType() == ItemTypeTagClass){				

				m_strName = ptr->GetName();

				many val;				
				m_nID = 0;
				if(ptr->GetPropertyValue(TagClassPropGuid, val))
					ATLVERIFY(val.cast(m_nID));
				if(ptr->GetPropertyValue(TagClassPropColAddress, val))
					m_strColAddr.Format(L"%s", val.to_string().c_str());
	
				ATLVERIFY(_SetComboValue(m_wndCmbType, ptr, TagClassPropType, &CItemTagClass::Type2String));
				ATLVERIFY(_SetComboValue(m_wndCmbClass, ptr, TagClassPropClass, &CItemTagClass::Class2String));
				ATLVERIFY(_SetComboValue(m_wndCmbAssignment, ptr, TagClassPropAssignment, &CItemTagClass::Assignment2String));
				ATLVERIFY(_SetComboValue(m_wndCmbUnitType, ptr, TagClassPropUnitsType, &CItemTagClass::UnitsType2String));
				ATLVERIFY(_SetComboValue(m_wndCmbUnits, ptr, TagClassPropUnits, &CItemTagClass::Units2String));

			}

		//	_EnableWindow(IDC_EDIT_NAME, false);

		}
		else {
			m_strName = CResourceHolder::GetControlTextUnavailableStr();

			many val;				
			m_nID = 0;
			bool bDiff = false;

			if(GetModel().GetMultiItemPropertyValue(m_rvctItemIDs, TagClassPropColAddress, val, bDiff)){
				if(!bDiff)
					m_strColAddr = val.to_string().c_str();
				else
					m_strColAddr = CResourceHolder::GetControlTextUnavailableStr();
			}

			ATLVERIFY(_SetComboValueMult(m_wndCmbType, TagClassPropType, &CItemTagClass::Type2String));
			ATLVERIFY(_SetComboValueMult(m_wndCmbClass, TagClassPropClass, &CItemTagClass::Class2String));
			ATLVERIFY(_SetComboValueMult(m_wndCmbAssignment, TagClassPropAssignment, &CItemTagClass::Assignment2String));
			ATLVERIFY(_SetComboValueMult(m_wndCmbUnitType, TagClassPropUnitsType, &CItemTagClass::UnitsType2String));
			ATLVERIFY(_SetComboValueMult(m_wndCmbUnits, TagClassPropUnits, &CItemTagClass::Units2String));

			// Disable some controls
			_EnableWindow(IDC_EDIT_NAME, false);
			_EnableWindow(IDC_EDIT_UID, false);
			//wndControl = GetDlgItem(IDC_EDT_FULL_NAME);
			//ATLASSERT(wndControl.IsWindow());
			//wndControl.EnableWindow(FALSE);
		}
		DoDataExchange(DDX_LOAD);
	}

	void _InitCombo(CComboBox& rBox, int nStart, int nEnd, boost::function<void (int, CString&)> f) 
	{
		int nItem = 0;		
		if(!IsSingleItemSelected())	{
			nItem = rBox.AddString(CResourceHolder::GetControlTextUnavailableStr());
			rBox.SetItemData(nItem, DIFF_ARGS_IDX);
		}		
		for (auto i = nStart; i <= nEnd ;i++){
			CString str;
			f(i, str);
			nItem = rBox.AddString(str);
			rBox.SetItemData(nItem, i);
		}
	}
	bool _SetComboValue(CComboBox& rBox, IItemPtr& ptr, int nPropID, boost::function<void (int, CString&)> f){
		int nData = DIFF_ARGS_IDX;
		many val;
		if(ptr->GetPropertyValue(nPropID, val)){			
			if(val.cast(nData)){
				CString strVal;
				f(nData, strVal);
				rBox.SelectString(0, strVal);
				return true;
			}
		}
		return false;
	}	
	bool _SetComboValueMult(CComboBox& rBox, int nPropID, boost::function<void (int, CString&)> f){
		int nData = DIFF_ARGS_IDX;
		bool bDiff = FALSE;
		many val;
		if(GetModel().GetMultiItemPropertyValue(m_rvctItemIDs, nPropID, val, bDiff)){		
			if(bDiff){
				rBox.SelectString(0, CResourceHolder::GetControlTextUnavailableStr());
				return true;
			}
			if(val.cast(nData)){
				CString strVal;
				f(nData, strVal);
				rBox.SelectString(0, strVal);
				return true;
			}
		}
		return false;
	}	
	bool _EnableWindow(UINT nID, bool bEnable = true){
		CWindow wndControl;
		wndControl = GetDlgItem(nID);
		ATLASSERT(wndControl.IsWindow());
		return wndControl.EnableWindow(bEnable);
	}

protected:
	const std::vector<int>& m_rvctItemIDs;
	int m_nChanged;

protected:
	CString m_strName;
	CString m_strColAddr;
	int m_nID;
	
protected:
	CComboBox m_wndCmbType;
	CComboBox m_wndCmbClass;
	CComboBox m_wndCmbAssignment;
	CComboBox m_wndCmbCharact;
	CComboBox m_wndCmbUnitType;
	CComboBox m_wndCmbUnits;

	CComboBox m_wndCmbDirection;
	CComboBox m_wndCmbTariff;

};
;