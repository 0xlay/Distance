#pragma once


class PowerManagerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PowerManagerDlg)

public:
	PowerManagerDlg(
		std::shared_ptr<Distance::Network::DefaultTcpServer> server,
		size_t clientID,
		CWnd* pParent = nullptr
	);
	virtual ~PowerManagerDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POWER_MANAGER_DIALOG };
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLogout();
	afx_msg void OnBnClickedShutDown();
	afx_msg void OnBnClickedRestart();

private:
	std::shared_ptr<Distance::Network::DefaultTcpServer> server_;
	size_t clientID_;
};
