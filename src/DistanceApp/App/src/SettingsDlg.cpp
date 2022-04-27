//------------------------------------------------------------------------------
// ╔═══╗╔══╗╔═══╗╔════╗╔═══╗╔═╗─╔╗╔═══╗╔═══╗
// ╚╗╔╗║╚╣─╝║╔═╗║║╔╗╔╗║║╔═╗║║║╚╗║║║╔═╗║║╔══╝
// ─║║║║─║║─║╚══╗╚╝║║╚╝║║─║║║╔╗╚╝║║║─╚╝║╚══╗
// ─║║║║─║║─╚══╗║──║║──║╚═╝║║║╚╗║║║║─╔╗║╔══╝
// ╔╝╚╝║╔╣─╗║╚═╝║──║║──║╔═╗║║║─║║║║╚═╝║║╚══╗
// ╚═══╝╚══╝╚═══╝──╚╝──╚╝─╚╝╚╝─╚═╝╚═══╝╚═══╝
// 
// Copyright Ⓒ 0xlay. All rights reserved.
// 
// This source file is licensed under the terms of MIT license.
// For details, please read the LICENSE file.
// 
// File: SettingsDlg.cpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceApp.exe
//
//------------------------------------------------------------------------------

#include "pch.hpp"
#include "App.hpp"
#include "SettingsDlg.hpp"
#include "afxdialogex.h"
#include "ServerSettings.hpp"


namespace Distance
{

    IMPLEMENT_DYNAMIC(SettingsDlg, CDialogEx)

    SettingsDlg::SettingsDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_SETTINGS_DIALOG, pParent), port_(_T(""))
    { }

    SettingsDlg::~SettingsDlg()
    { }

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

    BOOL SettingsDlg::OnInitDialog()
    {
        CDialogEx::OnInitDialog();

        using namespace Distance::Network;
        using namespace Distance::Utils;

        auto settings = std::make_shared<ServerSettings>();
        SettingsController::invoke()
            .load(std::reinterpret_pointer_cast<Settings>(settings));

        protocols_.AddString(L"IPV4");
        protocols_.AddString(L"IPV6");
        if (settings->getProtocol() == InternetProtocol::IPV4)
        {
            protocols_.SelectString(0, L"IPV4");
        }
        else
        {
            protocols_.SelectString(1, L"IPV6");
        }

        port_.Format(L"%d", settings->getPort());
        UpdateData(FALSE);
        // SetIcon(m_hIcon, TRUE); // Set big icon
        // SetIcon(m_hIcon, FALSE); // Set small icon

        // TODO: Add extra initialization here

        return TRUE;  // return TRUE  unless you set the focus to a control
    }

    void SettingsDlg::OnBnClickedSettingsSave()
    {
        UpdateData(TRUE);

        using namespace Distance::Network;
        using namespace Distance::Utils;

        auto settings = std::make_shared<ServerSettings>();

        if (protocols_.GetCurSel() == 0)
        {
            settings->setProtocol(InternetProtocol::IPV4);
        }
        else if (protocols_.GetCurSel() == 1)
        {
            settings->setProtocol(InternetProtocol::IPV6);
        }
        settings->setPort(std::stoi(port_.GetString()));

        SettingsController::invoke()
            .save(std::reinterpret_pointer_cast<Settings>(settings));

        EndDialog(ID_SETTINGS_SAVE);
    }

    void SettingsDlg::OnBnClickedSettingsCancel()
    {
        EndDialog(ID_SETTINGS_CANCEL);
    }

} // Distance