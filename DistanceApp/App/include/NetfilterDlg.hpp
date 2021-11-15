#pragma once


class NetfilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NetfilterDlg)

public:
	NetfilterDlg(CWnd* pParent = nullptr); // standard constructor
	virtual ~NetfilterDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NETFILTER_DIALOG };
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedSave();
private:
	CString log_;
public:
	afx_msg void OnBnClickedSaveAs();
};
