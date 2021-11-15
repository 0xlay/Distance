// ScreenshotDlg.cpp : implementation file
//

#include "pch.hpp"

#include "DistanceApp.hpp"
#include "ScreenshotDlg.hpp"
#include "afxdialogex.h"


// ScreenshotDlg dialog

IMPLEMENT_DYNAMIC(ScreenshotDlg, CDialogEx)

ScreenshotDlg::ScreenshotDlg(
	std::shared_ptr<Distance::Network::DefaultTcpServer> server,
	size_t clientID,
	CWnd* pParent /*=nullptr*/)
	: server_(std::move(server)), clientID_(clientID), CDialogEx(IDD_SCREENSHOT_DIALOG, pParent)
{ }

ScreenshotDlg::~ScreenshotDlg()
{ }

void ScreenshotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCREENSHOT_STATIC, image_);
}


BEGIN_MESSAGE_MAP(ScreenshotDlg, CDialogEx)
	ON_BN_CLICKED(ID_SCREENSHOT_CREATE, &ScreenshotDlg::OnBnClickedCreate)
	ON_BN_CLICKED(ID_SCREENSHOT_SAVE, &ScreenshotDlg::OnBnClickedSave)
	ON_BN_CLICKED(ID_SCREENSHOT_SAVEAS, &ScreenshotDlg::OnBnClickedSaveAs)
END_MESSAGE_MAP()



void ScreenshotDlg::OnBnClickedCreate()
{
	using namespace Distance::Network;
	MessageHeader header{ ServerMsgType::GetScreenshot, 0 };

	// Send the query GetProcessList to the client
	if (!server_->send(header, clientID_))
	{
		MessageBox(
			$T"Send message to the client was failed! "
			$T"The client may have been disconnected from the server.",
			$T"Distance", MB_ICONERROR
		);

		EndDialog(FAILED_CONNECTION_TO_CLIENT);
		return;
	}

	// Get process list from the client
	if (!server_->receiveAll(message_, clientID_))
	{
		MessageBox(
			$T"Receive message from the client was failed! "
			$T"The client may have been disconnected from the server.",
			$T"Distance", MB_ICONERROR
		);

		EndDialog(FAILED_CONNECTION_TO_CLIENT);
		return;
	}

	std::string bmpImage;
	message_ >> bmpImage; // @BUG: fix copy from std::vector to std::string in "Message"

	const auto fileHeader = (const PBITMAPFILEHEADER)(bmpImage.data());
	const auto infoHeader = (const PBITMAPINFOHEADER)(bmpImage.data() + sizeof(BITMAPFILEHEADER));

	auto lpBits = static_cast<LPVOID>(bmpImage.data() + fileHeader->bfOffBits);
	image_.SetBitmap(CreateBitmap(infoHeader->biWidth, infoHeader->biHeight, infoHeader->biPlanes, infoHeader->biBitCount, lpBits));

	UpdateData(FALSE);
}
                                                                                                                      

void ScreenshotDlg::OnBnClickedSave()
{
	// TODO: Add your control notification handler code here
}


void ScreenshotDlg::OnBnClickedSaveAs()
{
	// TODO: Add your control notification handler code here
}


void ScreenshotDlg::saveToFile()
{

}