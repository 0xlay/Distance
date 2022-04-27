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
// File: PowerManagerDlg.hpp
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

    class PowerManagerDlg : public CDialogEx
    {
        DECLARE_DYNAMIC(PowerManagerDlg)

    public:
        PowerManagerDlg(
            std::shared_ptr<Distance::Network::DefaultTcpServer> server,
            size_t clientID,
            CWnd* pParent = nullptr
        );
        virtual ~PowerManagerDlg();

        // Dialog Data
#ifdef AFX_DESIGN_TIME
        enum { IDD = IDD_POWER_MANAGER_DIALOG };
#endif

        afx_msg void OnBnClickedShutDown();
        afx_msg void OnBnClickedRestart();

    protected:
        void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV support
        DECLARE_MESSAGE_MAP()

    private:
        std::shared_ptr<Distance::Network::DefaultTcpServer> server_;
        size_t clientID_;
        Network::Message message_;
    };

} // Distance
