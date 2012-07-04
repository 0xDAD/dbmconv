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
		cPropAddress = 0x0200
	};

protected:
	BEGIN_MSG_MAP(CPropertyPageTagClass)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		//COMMAND_HANDLER(IDC_EDT_NAME, EN_CHANGE, OnChangeEditName)
		//COMMAND_HANDLER(IDC_EDT_DESCRIPTION, EN_CHANGE, OnChangeEditDescription)
		//COMMAND_HANDLER(IDC_CHK_DISABLED, BN_CLICKED, OnChangeCheckDisabled)
		CHAIN_MSG_MAP(baseResizeClass)
		CHAIN_MSG_MAP(baseClass)
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
		//DLGRESIZE_CONTROL(IDC_EDT_DESCRIPTION, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		//DLGRESIZE_CONTROL(IDC_CHK_DISABLED, DLSZ_MOVE_Y)
	END_DLGRESIZE_MAP()

	BEGIN_DDX_MAP(CPropertyPageTagClass)
		DDX_TEXT(IDC_EDIT_NAME, m_strName)
		DDX_INT(IDC_EDIT_UID, m_nID)

		DDX_CONTROL_HANDLE(IDC_COMBO_TYPE, m_wndCmbType)
		DDX_CONTROL_HANDLE(IDC_COMBO_CLASS, m_wndCmbClass)
		DDX_CONTROL_HANDLE(IDC_COMBO_FAMILY, m_wndCmbAssignment)
		DDX_CONTROL_HANDLE(IDC_COMBO_DIRECTION, m_wndCmbDirection)
		DDX_CONTROL_HANDLE(IDC_COMBO_TARIFF, m_wndCmbTariff)
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
		InitCombo(m_wndCmbType, TagTypeInvalid, TagTypeEvent, &CItemTagClass::Type2String);
		InitCombo(m_wndCmbAssignment, TagAssignmentNone, TagAssignment96, &CItemTagClass::Assignment2String);
		InitCombo(m_wndCmbClass, TagClassInvalid, TagClassBase2, &CItemTagClass::Class2String);
		/*InitCombo(m_wndCmbType, TagTypeInvalid, TagTypeEvent, &CItemTagClass::Type2String);
		InitCombo(m_wndCmbType, TagTypeInvalid, TagTypeEvent, &CItemTagClass::Type2String);
		InitCombo(m_wndCmbType, TagTypeInvalid, TagTypeEvent, &CItemTagClass::Type2String);*/
		InitData();
		m_nChanged = 0;
		SetModified(FALSE);
		return TRUE;
	}
	//LRESULT OnChangeEditName(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//{
	//	m_nChanged |= cName;
	//	SetModified();
	//	return 0;
	//}
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


	void InitCombo(CComboBox& rBox, int nStart, int nEnd, boost::function<void (int, CString&)> f) 
	{
		int nItem = 0;
		CString str;
		if(!IsSingleItemSelected())	{
			nItem = rBox.AddString(str);
			rBox.SetItemData(nItem, DIFF_ARGS_IDX);
		}		
		for (auto i = nStart; i <= nEnd ;i++){
			f(i, str);
			nItem = rBox.AddString(str);
			rBox.SetItemData(nItem, i);
		}
	}

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
					
				if(ptr->GetPropertyValue(TagClassPropType, val)){
					int nType = -2;
					if(val.cast(nType)){
						CString strVal;
						CItemTagClass::Type2String(nType, strVal);
						m_wndCmbType.SelectString(0, strVal);
					}
				}
				if(ptr->GetPropertyValue(TagClassPropClass, val)){
					int nClass = -2;
					if(val.cast(nClass)){
						CString strVal;
						CItemTagClass::Class2String(nClass, strVal);
						m_wndCmbClass.SelectString(0, strVal);
					}
				}
				if(ptr->GetPropertyValue(TagClassPropAssignment, val)){
					int nAss = -2;
					if(val.cast(nAss)){
						CString strVal;
						CItemTagClass::Assignment2String(nAss, strVal);
						m_wndCmbAssignment.SelectString(0, strVal);
					}
				}
			}
			/*if (GetNamespace().GetItem(m_rvctItemIDs.front(), &spItem)) {
			m_strName = spItem->GetName();
			m_strFullName  = GetNamespace().GetItemFullNameSimple(m_rvctItemIDs.front());
			m_strDescription = spItem->GetDescription();
			m_nDisabled = spItem->IsDisabled()? 1 : 0;

			CEdit wndEdtControl;
			wndEdtControl = GetDlgItem(IDC_EDT_NAME);
			ATLASSERT(wndEdtControl.IsWindow());
			if (GetNamespace().CanModifyItem(spItem->GetID(), OPCTL::ItemPropName))
			ATLVERIFY(wndEdtControl.SetReadOnly(FALSE));
			else
			ATLVERIFY(wndEdtControl.SetReadOnly(TRUE));

			wndEdtControl = GetDlgItem(IDC_EDT_DESCRIPTION);
			ATLASSERT(wndEdtControl.IsWindow());
			if (GetNamespace().CanModifyItem(spItem->GetID(), OPCTL::ItemPropDescription))
			ATLVERIFY(wndEdtControl.SetReadOnly(FALSE));
			else
			ATLVERIFY(wndEdtControl.SetReadOnly(TRUE));
			}*/
		}
		else {
			m_strName = CResourceHolder::GetControlTextUnavailableStr();
			//m_strFullName = CResourceHolder::GetControlTextUnavailableStr();

			//CComVariant vValue;
			//bool bDifferent = false;

			//ATLVERIFY(GetNamespace().GetMultiItemPropertyValueStr(m_rvctItemIDs, OPCTL::ItemPropDescription, m_strDescription, OPCTL::ItemPropGetTypeAlways, bDifferent));
			//ATLVERIFY(GetNamespace().GetMultiItemPropertyValue(m_rvctItemIDs, OPCTL::ItemPropDisabled, vValue, OPCTL::ItemPropGetTypeAlways, bDifferent));
			//if (bDifferent || FAILED(vValue.ChangeType(VT_BOOL)))
			//	m_nDisabled = 2;
			//else
			//	m_nDisabled = (V_BOOL(&vValue) != VARIANT_FALSE)? 1: 0;

			//// Disable some controls
			//CWindow wndControl;
			//wndControl = GetDlgItem(IDC_EDT_NAME);
			//ATLASSERT(wndControl.IsWindow());
			//wndControl.EnableWindow(FALSE);

			//wndControl = GetDlgItem(IDC_EDT_FULL_NAME);
			//ATLASSERT(wndControl.IsWindow());
			//wndControl.EnableWindow(FALSE);
		}
		DoDataExchange(DDX_LOAD);
	}

protected:
	const std::vector<int>& m_rvctItemIDs;
	int m_nChanged;
protected:
	CString m_strName;
	int m_nID;
	
protected:
	CComboBox m_wndCmbType;
	CComboBox m_wndCmbClass;
	CComboBox m_wndCmbAssignment;
	CComboBox m_wndCmbDirection;
	CComboBox m_wndCmbTariff;
};
