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
// File: NetfilterDlg.hpp
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

    class NetfilterDlg : public CDialogEx
    {
        DECLARE_DYNAMIC(NetfilterDlg)

    public:
        NetfilterDlg(std::shared_ptr<Network::DefaultTcpServer> server,
                     size_t clientID,
                     CWnd* pParent = nullptr);
        virtual ~NetfilterDlg();

        // Dialog Data
#ifdef AFX_DESIGN_TIME
        enum { IDD = IDD_NETFILTER_DIALOG };
#endif

        afx_msg void OnBnClickedBlockHttp();
        afx_msg void OnBnClickedUnblockHttp();

    protected:
        void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV support
        DECLARE_MESSAGE_MAP()

    private:
        Network::Message message_;
        std::shared_ptr<Network::DefaultTcpServer> server_;
        size_t clientID_;
    };

} // Distance
