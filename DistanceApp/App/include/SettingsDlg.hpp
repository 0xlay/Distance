#pragma once


class SettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SettingsDlg)

public:
	SettingsDlg(CWnd* pParent = nullptr); // standard constructor
	virtual ~SettingsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGS_DIALOG };
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV support

	// Generated message map functions
	BOOL OnInitDialog() override;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSettingsSave();
	afx_msg void OnBnClickedSettingsCancel();

private:
	CString port_;
	CComboBox protocols_;
};
