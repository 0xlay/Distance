// SettingsDlg.cpp : implementation file
//

#include "pch.hpp"

#include "DistanceApp.hpp"
#include "SettingsDlg.hpp"
#include "afxdialogex.h"

#include "ServerSettings.hpp"
//#include "Server.hpp"


// SettingsDlg dialog

IMPLEMENT_DYNAMIC(SettingsDlg, CDialogEx)

SettingsDlg::SettingsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTINGS_DIALOG, pParent)
	, port_(_T(""))
{
}

SettingsDlg::~SettingsDlg()
{
}

void SettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SETTINGS_EDIT_PORT, port_);
	DDX_Control(pDX, IDC_SETTINGS_PROTOCOL_COMBO, protocols_);
}


BEGIN_MESSAGE_MAP(SettingsDlg, CDialogEx)
	ON_BN_CLICKED(ID_SETTINGS_SAVE, &SettingsDlg::OnBnClickedSettingsSave)
	ON_BN_CLICKED(ID_SETTINGS_CANCEL, &SettingsDlg::OnBnClickedSettingsCancel)
END_MESSAGE_MAP()


// SettingsDlg message handlers

BOOL SettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	using namespace Distance::Network;
	ServerSettings settings;
	Configurator::invoke().loadSettings(&settings);

	protocols_.AddString(_T("IPV4"));
	protocols_.AddString(_T("IPV6"));
	if (settings.getProtocol() == InternetProtocol::IPV4)
	{
		protocols_.SelectString(0, _T("IPV4"));
	}
	else
	{
		protocols_.SelectString(1, _T("IPV6"));
	}

	port_.Format(_T("%d"), settings.getPort());
	UpdateData(FALSE);
	// SetIcon(m_hIcon, TRUE);			// Set big icon
	// SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void SettingsDlg::OnBnClickedSettingsSave()
{
	UpdateData(TRUE);

	using namespace Distance::Network;
	ServerSettings settings;

	if (protocols_.GetCurSel() == 0)
	{
		settings.setProtocol(InternetProtocol::IPV4);
	}
	else if(protocols_.GetCurSel() == 1)
	{
		settings.setProtocol(InternetProtocol::IPV6);
	}
	settings.setPort(std::stoi(port_.GetString()));

	Configurator::invoke().saveSettings(&settings);

	EndDialog(ID_SETTINGS_SAVE);
}


void SettingsDlg::OnBnClickedSettingsCancel()
{
	EndDialog(ID_SETTINGS_CANCEL);
}
