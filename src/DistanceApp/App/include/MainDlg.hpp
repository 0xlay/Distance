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
// File: MainDlg.hpp
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
    class MainDlg;

    namespace _internal
    {

        void WaitableTimer(MainDlg& mainDlg);

    } // Distance::_internal

    class MainDlg : public CDialogEx
    {
    public:
        MainDlg(
            std::shared_ptr<Network::DefaultTcpServer> server,
            Network::Message& msg,
            CWnd* pParent = nullptr
        );

        ~MainDlg();

#ifdef AFX_DESIGN_TIME
        enum { IDD = IDD_MAIN_DIALOG };
#endif

        afx_msg void OnHelpAbout();
        afx_msg void OnSettings();
        afx_msg void OnBnClickedButtonProcessManager();
        afx_msg void OnBnClickedButtonPowerManager();
        afx_msg void OnBnClickedButtonScreenshot();
        afx_msg void OnBnClickedButtonKeylogger();
        afx_msg void OnBnClickedButtonNetfilter();
        afx_msg void OnBnClickedButtonSysinfo();

    private:
        void addPcToList();
        void removePcFromList();

        friend void _internal::WaitableTimer(MainDlg& mainDlg);

        void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV support

        BOOL OnInitDialog() override;
        afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
        afx_msg void OnPaint();
        afx_msg HCURSOR OnQueryDragIcon();
        DECLARE_MESSAGE_MAP()

    private:
        std::shared_ptr<Network::DefaultTcpServer> server_;
        Network::Message& message_;
        std::mutex mutex_;

        size_t counterPC_;
        CListCtrl listPC_;
        CImageList images_;
        HICON mainIcon_;

        HANDLE timer_{};
        std::atomic<bool> isTimerRunning_{};
        std::unique_ptr<std::thread> waitableThread_;
        enum
        {
            PeriodInMS = 1000,
            MaxTimeoutInMS = 1500
        };
    };

} // Distance