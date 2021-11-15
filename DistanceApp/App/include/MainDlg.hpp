
// MainDlg.h : header file
//

#pragma once

class MainDlg : public CDialogEx
{
// Construction
public:
	MainDlg(
		std::shared_ptr<Distance::Network::DefaultTcpServer> server,
		Distance::Network::Message& msg,
		CWnd* pParent = nullptr
	);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_DIALOG };
#endif

private:
	std::shared_ptr<Distance::Network::DefaultTcpServer> server_;
	Distance::Network::Message& message_;
	size_t counterPC_;

	CListCtrl listPC_;
	CImageList images_;

private:
	void addPcToList();
	void removePcFromList();

protected:
	void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	BOOL OnInitDialog() override;
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnHelpAbout();
	afx_msg void OnSettings();
	afx_msg void OnBnClickedButtonListUpdate();
	afx_msg void OnBnClickedButtonProcessManager();
	afx_msg void OnBnClickedButtonPowerManager();
	afx_msg void OnBnClickedButtonScreenshot();
	afx_msg void OnBnClickedButtonKeylogger();
	afx_msg void OnBnClickedButtonNetfilter();
	afx_msg void OnBnClickedButtonSysinfo();
};
