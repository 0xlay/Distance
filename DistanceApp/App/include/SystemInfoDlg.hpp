#pragma once
// SystemInfoDlg dialog

class SystemInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SystemInfoDlg)

public:
	SystemInfoDlg(
		std::shared_ptr<Distance::Network::DefaultTcpServer> server,
		size_t clientID, 
		CWnd* pParent = nullptr
	); // standard constructor
	
	virtual ~SystemInfoDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SYSINFO_DIALOG };
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV support
	BOOL OnInitDialog() override;
	DECLARE_MESSAGE_MAP()

private:
	CString localName_	= $T"";
	CString ip_			= $T"";
	CString ram_		= $T"";
	CString cpu_		= $T"";

	std::shared_ptr<Distance::Network::DefaultTcpServer> server_;
	size_t clientID_;

	Distance::Network::Message message_;
};
