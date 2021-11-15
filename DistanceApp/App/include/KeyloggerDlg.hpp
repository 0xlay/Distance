#pragma once


class KeyloggerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(KeyloggerDlg)

public:
	KeyloggerDlg(CWnd* pParent = nullptr); // standard constructor
	virtual ~KeyloggerDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KEYLOGGER_DIALOG };
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedSaveAs();
};
