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
// File: SystemInfoDlg.hpp
// 
// Creator: 0xlay
// 
// Environment: User mode only
//
// Module: DistanceApp.exe
//
//------------------------------------------------------------------------------

#pragma once


namespace Distance
{

    class SystemInfoDlg : public CDialogEx
    {
        DECLARE_DYNAMIC(SystemInfoDlg)

    public:
        SystemInfoDlg(
            std::shared_ptr<Network::DefaultTcpServer> server,
            size_t clientID,
            CWnd* pParent = nullptr
        );

        ~SystemInfoDlg();

        // Dialog Data
#ifdef AFX_DESIGN_TIME
        enum { IDD = IDD_SYSINFO_DIALOG };
#endif

    protected:
        void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV support
        BOOL OnInitDialog() override;
        DECLARE_MESSAGE_MAP()

    private:
        CStringW localName_ = L"";
        CStringW ip_        = L"";
        CStringW ram_       = L"";
        CStringW cpu_       = L"";

        std::shared_ptr<Network::DefaultTcpServer> server_;
        size_t clientID_;

        Network::Message message_;
    };

} // Distance