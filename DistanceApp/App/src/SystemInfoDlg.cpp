// SystemInfoDlg.cpp : implementation file
//

#include "pch.hpp"

#include "DistanceApp.hpp"
#include "SystemInfoDlg.hpp"
#include "afxdialogex.h"

// SystemInfoDlg dialog


IMPLEMENT_DYNAMIC(SystemInfoDlg, CDialogEx)

SystemInfoDlg::SystemInfoDlg(
	std::shared_ptr<Distance::Network::DefaultTcpServer> server,
	size_t clientID, 
	CWnd* pParent /*=nullptr*/
)
	: server_(std::move(server)), clientID_(clientID), CDialogEx(IDD_SYSINFO_DIALOG, pParent)
{ }

SystemInfoDlg::~SystemInfoDlg()
{ }

void SystemInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOCALNAME, localName_);
	DDX_Text(pDX, IDC_EDIT_IP, ip_);
	DDX_Text(pDX, IDC_EDIT_RAM, ram_);
	DDX_Text(pDX, IDC_EDIT_CPU, cpu_);
}


BEGIN_MESSAGE_MAP(SystemInfoDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL SystemInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Distance::Network::MessageHeader header{ Distance::Network::ServerMsgType::GetPcInfo, 0 };

	if (!server_->send(header, clientID_))
	{
		MessageBox(
			$T"Send message to the client was failed! "
			$T"The client may have been disconnected from the server.",
			$T"Distance", MB_ICONERROR
		);

		EndDialog(FAILED_CONNECTION_TO_CLIENT);
		return FALSE;
	}

	if (!server_->receiveAll(message_, clientID_))
	{
		MessageBox(
			$T"Receive message from the client was failed! "
			$T"The client may have been disconnected from the server.",
			$T"Distance", MB_ICONERROR
		);

		EndDialog(FAILED_CONNECTION_TO_CLIENT);
		return FALSE;
	}

	using namespace Distance::Network;
	Json::PcInfo info;
	std::string result;

	message_ >> result;
	Json::unpack(result, info);

	localName_ = info.localName.c_str();
	ip_ = info.ip.c_str();
	ram_.Format(
		$T"Total: %llu MB, Available: %llu MB, Loaded: %d percents",
		(info.ram.TotalMemory / 1024 / 1024), (info.ram.AvailMemory / 1024 / 1024), info.ram.MemoryLoad
	);

	std::ostringstream oss;
	oss << info.cpu.architecture << " " << info.cpu.name;
	cpu_ = oss.str().c_str();

	UpdateData(FALSE);
	return TRUE;
}

// SystemInfoDlg message handlers
