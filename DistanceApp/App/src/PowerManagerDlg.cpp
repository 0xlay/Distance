// PowerManagerDlg.cpp : implementation file
//

#include "pch.hpp"

#include "DistanceApp.hpp"
#include "PowerManagerDlg.hpp"
#include "afxdialogex.h"


IMPLEMENT_DYNAMIC(PowerManagerDlg, CDialogEx)

PowerManagerDlg::PowerManagerDlg(
	std::shared_ptr<Distance::Network::DefaultTcpServer> server,
	size_t clientID,
	CWnd* pParent /*nullptr*/
)
	: server_(std::move(server)), clientID_(clientID), CDialogEx(IDD_POWER_MANAGER_DIALOG, pParent)
{ }

PowerManagerDlg::~PowerManagerDlg()
{ }

void PowerManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(PowerManagerDlg, CDialogEx)
	ON_BN_CLICKED(ID_POWER_MANAGER_SLEEP, &PowerManagerDlg::OnBnClickedLogout)
	ON_BN_CLICKED(ID_POWER_MANAGER_SHUT_DOWN, &PowerManagerDlg::OnBnClickedShutDown)
	ON_BN_CLICKED(ID_POWER_MANAGER_RESTART, &PowerManagerDlg::OnBnClickedRestart)
END_MESSAGE_MAP()

void PowerManagerDlg::OnBnClickedLogout()
{
	using namespace Distance::Network;
	MessageHeader header{ ServerMsgType::LogoutPC, 0 };

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
}

void PowerManagerDlg::OnBnClickedShutDown()
{
	using namespace Distance::Network;
	MessageHeader header{ ServerMsgType::ShutdownPC, 0 };

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
}

void PowerManagerDlg::OnBnClickedRestart()
{
	using namespace Distance::Network;
	MessageHeader header{ ServerMsgType::RebootPC, 0 };

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
}
