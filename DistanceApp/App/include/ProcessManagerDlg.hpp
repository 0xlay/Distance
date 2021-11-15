#pragma once


class ProcessManagerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ProcessManagerDlg)

public:
	ProcessManagerDlg(
		std::shared_ptr<Distance::Network::DefaultTcpServer> server,
		size_t clientID,
		CWnd* pParent = nullptr
	);
	virtual ~ProcessManagerDlg();

	afx_msg void OnHdnClickProcessManagerList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedTerminate();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedSaveAs();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESS_MANAGER_DIALOG };
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnInitDialog() override;
	DECLARE_MESSAGE_MAP()

private:
	friend int CALLBACK Compare(LPARAM lhs, LPARAM rhs, LPARAM pThis);
	void saveToFile();

private:
	Distance::Network::Message message_;
	std::shared_ptr<Distance::Network::DefaultTcpServer> server_;
	CListCtrl list_;
	size_t clientID_;

	Distance::Network::Json::ProcessList processList_;

	CString fileName_;

	int sortColumn_{};
	bool isSortAscending{ true };
};

int CALLBACK Compare(LPARAM lhs, LPARAM rhs, LPARAM pThis);