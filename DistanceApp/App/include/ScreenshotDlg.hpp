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

	~ScreenshotDlg() override;

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCREENSHOT_DIALOG };
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCreate();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedSaveAs();

private:
	void saveToFile();

private:
	CStatic image_;
	CString fileName_;

	Distance::Network::Message message_;
	std::shared_ptr<Distance::Network::DefaultTcpServer> server_;
	size_t clientID_;
};
