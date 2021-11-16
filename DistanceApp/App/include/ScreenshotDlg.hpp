#pragma once


class ScreenshotDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ScreenshotDlg)

public:
	ScreenshotDlg(
		std::shared_ptr<Distance::Network::DefaultTcpServer> server,
		size_t clientID,
		CWnd* pParent = nullptr
	);

	~ScreenshotDlg() override = default;

	afx_msg void OnBnClickedCreate();
	afx_msg void OnBnClickedOpenFolder();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCREENSHOT_DIALOG };
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnInitDialog() override;
	DECLARE_MESSAGE_MAP()

private:
	xstar::tstring folderName_;

	Distance::Network::Message message_;
	std::shared_ptr<Distance::Network::DefaultTcpServer> server_;
	size_t clientID_;
};
